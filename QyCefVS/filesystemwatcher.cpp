#include "filesystemwatcher.h"
#include <QDir>
#include <QFileInfo>
#include <QDebug>

/// <summary>
/// 构造函数
/// </summary>
/// <param name="parent"></param>
FileSystemWatcher::FileSystemWatcher(QObject* parent) :QObject(parent) {
	m_pSystemWatcher = new QFileSystemWatcher();
	// 连接QFileSystemWatcher的directoryChanged和fileChanged信号到相应的槽
	connect(m_pSystemWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(directoryUpdated(QString)));
	connect(m_pSystemWatcher, SIGNAL(fileChanged(QString)), this, SLOT(fileUpdated(QString)));
}
// 监控文件或目录
void FileSystemWatcher::addWatchPath(QString path)
{
	qDebug() << QString("Add to watch: %1").arg(path);
	// 添加监控路径
	m_pSystemWatcher->addPath(path);

	// 如果添加路径是一个目录，保存当前内容列表
	QFileInfo file(path);
	if (file.isDir())
	{
		const QDir dirw(path);
		m_currentContentsMap[path] = dirw.entryList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst);
	}
}

// 只要任何监控的目录更新（添加、删除、重命名），就会调用。
void FileSystemWatcher::directoryUpdated(const QString& path)
{
	// qDebug() << QString("Directory updated: %1").arg(path);
	// 比较最新的内容和保存的内容找出区别(变化)
	QStringList currEntryList = m_currentContentsMap[path];
	const QDir dir(path);

	QStringList newEntryList = dir.entryList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst);

	QSet<QString> newDirSet = QSet<QString>::fromList(newEntryList);
	QSet<QString> currentDirSet = QSet<QString>::fromList(currEntryList);

	// 添加了文件
	QSet<QString> newFiles = newDirSet - currentDirSet;
	QStringList newFile = newFiles.toList();
	// 文件已被移除
	QSet<QString> deletedFiles = currentDirSet - newDirSet;
	QStringList deleteFile = deletedFiles.toList();
	// 更新当前设置
	m_currentContentsMap[path] = newEntryList;
	if (!newFile.isEmpty() && !deleteFile.isEmpty())
	{
		// 文件/目录重命名
		if ((newFile.count() == 1) && (deleteFile.count() == 1))
		{
			FileChangeEvent fileChangeEvent = {
				FileChangeEventType::RENAME,
				QString("File Renamed from %1 to %2").arg(deleteFile.first()).arg(newFile.first())
			};
			// 触发事件
			emit onFileChangeEventTrigger(fileChangeEvent);
		}
	}
	else
	{
		// 添加新文件/目录至Dir
		if (!newFile.isEmpty())
		{
			FileChangeEvent fileChangeEvent = {
				FileChangeEventType::NEW,
				QString("New Files/Dirs added: %1").arg(newFile.first())
			};

			//qDebug() << "New Files/Dirs added: " << newFile;
			emit onFileChangeEventTrigger(fileChangeEvent);

			foreach(QString file, newFile)
			{
				// 新文件被监控
				//m_pSystemWatcher->addPath(path + "\\" + file);
			}
		}
		// 从Dir中删除文件/目录
		if (!deleteFile.isEmpty())
		{
			FileChangeEvent fileChangeEvent = {
				FileChangeEventType::REMOVE,
				QString("Files/Dirs deleted: %1").arg(deleteFile.first())
			};
			emit onFileChangeEventTrigger(fileChangeEvent);
			foreach(QString file, deleteFile)
			{
				// 处理操作每个被删除的文件....
				//m_pSystemWatcher->removePath(path + "\\" + file);
			}
		}
	}
}

// 文件修改时调用
void FileSystemWatcher::fileUpdated(const QString& path)
{
	QFileInfo file(path);
	QString strPath = file.absolutePath();
	QString strName = file.fileName();
	FileChangeEvent fileChangeEvent = {
				FileChangeEventType::UPDATE_FILE,
				QString("The file %1 at path %2 is updated").arg(strName).arg(strPath)
	};
	emit onFileChangeEventTrigger(fileChangeEvent);
	//qDebug() << QString("The file %1 at path %2 is updated").arg(strName).arg(strPath);
}
