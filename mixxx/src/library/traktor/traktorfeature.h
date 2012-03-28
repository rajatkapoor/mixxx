// traktorfeature.h
// Created 9/26/2010 by Tobias Rafreider

#ifndef TRAKTOR_FEATURE_H
#define TRAKTOR_FEATURE_H

#include <QStringListModel>
#include <QtSql>
#include <QXmlStreamReader>
#include <QFuture>
#include <QtConcurrentRun>
#include <QFutureWatcher>

#include "library/libraryfeature.h"
#include "library/traktor/traktortablemodel.h"
#include "library/traktor/traktorplaylistmodel.h"
#include "library/treeitemmodel.h"

class LibraryTableModel;
class MissingTableModel;
class TrackCollection;

class TraktorFeature : public LibraryFeature {
    Q_OBJECT
    public:
    TraktorFeature(QObject* parent, TrackCollection*);
    virtual ~TraktorFeature();

    QVariant title();
    QIcon getIcon();
    static bool isSupported();
    bool dropAccept(QUrl url);
    bool dropAcceptChild(const QModelIndex& index, QUrl url);
    bool dragMoveAccept(QUrl url);
    bool dragMoveAcceptChild(const QModelIndex& index, QUrl url);


    TreeItemModel* getChildModel();

public slots:
    void activate();
    void activateChild(const QModelIndex& index);
    void onRightClick(const QPoint& globalPos);
    void onRightClickChild(const QPoint& globalPos, QModelIndex index);
    void onLazyChildExpandation(const QModelIndex& index);
    void refreshLibraryModels();
    void onTrackCollectionLoaded();
private:
    TreeItem* importLibrary(QString file);
    /** parses a track in the music collection **/
    void parseTrack(QXmlStreamReader &xml, QSqlQuery &query);
    /** Iterates over all playliost and folders and constructs the childmodel **/
    TreeItem* parsePlaylists(QXmlStreamReader &xml);
    /** processes a particular playlist **/
    void parsePlaylistEntries(QXmlStreamReader &xml, QString playlist_path,
    QSqlQuery query_insert_into_playlist, QSqlQuery query_insert_into_playlisttracks);
    void clearTable(QString table_name);
    static QString getTraktorMusicDatabase();
    //private fields
    TreeItemModel m_childModel;
    TrackCollection* m_pTrackCollection;
    //A separate db connection for the worker parsing thread
    QSqlDatabase m_database;
    TraktorTableModel* m_pTraktorTableModel;
    TraktorPlaylistModel* m_pTraktorPlaylistModel;

    bool m_isActivated;
    bool m_cancelImport;
    QFutureWatcher<TreeItem*> m_future_watcher;
    QFuture<TreeItem*> m_future;
    QString m_title;
};



#endif /* TRAKTOR_FEATURE_H */