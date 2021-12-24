#ifndef FILESYSTEMWATCHER_H
#define FILESYSTEMWATCHER_H

#include <QObject>
#include <QMap>
#include <QFileSystemWatcher>

// 文件变化类型
typedef enum _FileChangeEventType {
	// 新建文件夹或文件，删除文件夹或文件，新建文件或文件夹，删除文件或文件夹，更新文件, 重命名文件或文件夹
	NEW, REMOVE, UPDATE_FILE, RENAME
} FileChangeEventType;

// 文件变化事件内容
typedef struct _FileChangeEvent
{

	FileChangeEventType fileChangeEventType; //事件类型
	QString message; //消息
}FileChangeEvent;


class FileSystemWatcher : public QObject
{
	Q_OBJECT
public:
	explicit FileSystemWatcher(QObject* parent = 0);
	void addWatchPath(QString path);

signals:
	// 文件改变信号
	void onFileChangeEventTrigger(FileChangeEvent event);

public slots:
	void directoryUpdated(const QString& path);  // 目录更新时调用，path是监控的路径
	void fileUpdated(const QString& path);   // 文件被修改时调用，path是监控的路径


private:
	QFileSystemWatcher* m_pSystemWatcher;  // QFileSystemWatcher变量
	QMap<QString, QStringList> m_currentContentsMap; // 当前每个监控的内容目录列表
};

#endif // FILESYSTEMWATCHER_H
