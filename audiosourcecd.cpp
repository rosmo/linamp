#include "audiosourcecd.h"

AudioSourceCD::AudioSourceCD(QObject *parent)
    : AudioSource{parent}
{
    Py_Initialize();
    PyEval_InitThreads();
    PyEval_SaveThread();

    auto state = PyGILState_Ensure();

    PyObject *pModuleName = PyUnicode_DecodeFSDefault("cdplayer");
    //PyObject *pModuleName = PyUnicode_DecodeFSDefault("mock_cdplayer");
    cdplayerModule = PyImport_Import(pModuleName);
    Py_DECREF(pModuleName);

    if(cdplayerModule == nullptr) {
        qDebug() << "Couldn't load python module";
        PyGILState_Release(state);
        return;
    }

    PyObject *CDPlayerClass = PyObject_GetAttrString(cdplayerModule, "CDPlayer");

    if(!CDPlayerClass || !PyCallable_Check(CDPlayerClass)) {
        qDebug() << "Error getting CDPlayer Class";
        PyGILState_Release(state);
        return;
    }

    cdplayer = PyObject_CallNoArgs(CDPlayerClass);

    // Timer to detect disc insertion and load
    detectDiscInsertionTimer = new QTimer(this);
    detectDiscInsertionTimer->setInterval(5000);
    connect(detectDiscInsertionTimer, &QTimer::timeout, this, &AudioSourceCD::pollDetectDiscInsertion);
    detectDiscInsertionTimer->start();

    // Watch for async disc detection results
    connect(&pollResultWatcher, &QFutureWatcher<bool>::finished, this, &AudioSourceCD::handlePollResult);

    PyGILState_Release(state);
}

AudioSourceCD::~AudioSourceCD()
{
    //stopPACapture();
    PyGILState_Ensure();
    Py_Finalize();
}

void AudioSourceCD::pollDetectDiscInsertion()
{
    if(pollInProgress) return;
    pollInProgress = true;
    qDebug() << "pollDetectDiscInsertion: polling";
    QFuture<bool> status = QtConcurrent::run(&AudioSourceCD::doPollDetectDiscInsertion, this);
    //pollStatus = &status;
    pollResultWatcher.setFuture(status);
}

void AudioSourceCD::handlePollResult()
{
    qDebug() << ">>>>POLL RESULT";
    bool discDetected = pollResultWatcher.result();
    if(discDetected) {
        refreshStatus();
    }
    pollInProgress = false;
}


bool AudioSourceCD::doPollDetectDiscInsertion()
{
    bool discDetected = false;
    if(cdplayer == nullptr) return discDetected;

    auto state = PyGILState_Ensure();
    PyObject* pyDiscDetected = PyObject_CallMethod(cdplayer, "detect_disc_insertion", NULL);

    if(PyBool_Check(pyDiscDetected)) {
        discDetected = PyObject_IsTrue(pyDiscDetected);

        qDebug() << ">>>Disct detected?:" << discDetected;
    } else {
        qDebug() << ">>>>pollDetectDiscInsertion: Not a bool";
    }
    if(pyDiscDetected) Py_DECREF(pyDiscDetected);
    PyGILState_Release(state);
    return discDetected;
}




void AudioSourceCD::activate()
{
    QMediaMetaData metadata = QMediaMetaData{};
    metadata.insert(QMediaMetaData::Title, "CD");

    emit playbackStateChanged(MediaPlayer::StoppedState);
    emit positionChanged(0);
    emit metadataChanged(metadata);
    emit durationChanged(0);
    emit eqEnabledChanged(false);
    emit plEnabledChanged(false);
    emit shuffleEnabledChanged(false);
    emit repeatEnabledChanged(false);

    refreshStatus();

    qDebug() << ">>>>CD Activated";
    // TODO

}

void AudioSourceCD::deactivate()
{
    //stopSpectrum();
    emit playbackStateChanged(MediaPlayer::StoppedState);
    if(cdplayer == nullptr) return;
    auto state = PyGILState_Ensure();
    PyObject_CallMethod(cdplayer, "stop", NULL);
    PyGILState_Release(state);
}

void AudioSourceCD::handlePl()
{

}

void AudioSourceCD::handlePrevious()
{
    if(cdplayer == nullptr) return;
    auto state = PyGILState_Ensure();
    PyObject_CallMethod(cdplayer, "prev", NULL);
    PyGILState_Release(state);
    refreshStatus();
}

void AudioSourceCD::handlePlay()
{
    if(cdplayer == nullptr) return;
    auto state = PyGILState_Ensure();
    PyObject_CallMethod(cdplayer, "play", NULL);
    PyGILState_Release(state);
    refreshStatus();
}

void AudioSourceCD::handlePause()
{
    if(cdplayer == nullptr) return;
    auto state = PyGILState_Ensure();
    PyObject_CallMethod(cdplayer, "pause", NULL);
    PyGILState_Release(state);
    refreshStatus();
}

void AudioSourceCD::handleStop()
{
    if(cdplayer == nullptr) return;
    auto state = PyGILState_Ensure();
    PyObject_CallMethod(cdplayer, "stop", NULL);
    PyGILState_Release(state);
    refreshStatus();
}

void AudioSourceCD::handleNext()
{
    if(cdplayer == nullptr) return;
    auto state = PyGILState_Ensure();
    PyObject_CallMethod(cdplayer, "next", NULL);
    PyGILState_Release(state);
    refreshStatus();
}

void AudioSourceCD::handleOpen()
{
    if(cdplayer == nullptr) return;
    auto state = PyGILState_Ensure();
    PyObject_CallMethod(cdplayer, "eject", NULL);
    PyGILState_Release(state);
    refreshStatus();
}

void AudioSourceCD::handleShuffle()
{

}

void AudioSourceCD::handleRepeat()
{

}

void AudioSourceCD::handleSeek(int mseconds)
{

}

void AudioSourceCD::refreshStatus()
{
    if(cdplayer == nullptr) return;
    auto state = PyGILState_Ensure();
    PyObject *pyStatus = PyObject_CallMethod(cdplayer, "get_status", NULL);
    if(pyStatus == nullptr) {
        PyGILState_Release(state);
        return;
    }
    QString status(PyUnicode_AsUTF8(pyStatus));
    Py_DECREF(pyStatus);
    PyGILState_Release(state);

    qDebug() << ">>>Status" << status;

    if(status == "no-disc") {
        QMediaMetaData metadata = QMediaMetaData{};
        metadata.insert(QMediaMetaData::Title, "NO DISC");
        emit metadataChanged(metadata);
        emit playbackStateChanged(MediaPlayer::StoppedState);
        emit positionChanged(0);
        emit durationChanged(0);
    }

    if(status == "stopped") {
        emit playbackStateChanged(MediaPlayer::StoppedState);
        emit positionChanged(0);
    }

    if(status == "playing") {
        emit playbackStateChanged(MediaPlayer::PlayingState);
    }

    if(status == "paused") {
        emit playbackStateChanged(MediaPlayer::PausedState);
    }

    if(status != "no-disc") {
        refreshTrackInfo();
    }
}

void AudioSourceCD::refreshTrackInfo()
{
    qDebug() << ">>>>>>>>>Refresh track info";
    if(cdplayer == nullptr) return;
    auto state = PyGILState_Ensure();
    PyObject *pyTrackInfo = PyObject_CallMethod(cdplayer, "get_current_track_info", NULL);
    if(pyTrackInfo == nullptr) {
        qDebug() << ">>> Couldn't get track info";
        PyErr_Print();
        PyGILState_Release(state);
        return;
    }
    // format (tracknumber: int, artist, album, title, duration: int, is_data_track: bool)
    PyObject *pyTrackNumber = PyTuple_GetItem(pyTrackInfo, 0);
    PyObject *pyArtist = PyTuple_GetItem(pyTrackInfo, 1);
    PyObject *pyAlbum = PyTuple_GetItem(pyTrackInfo, 2);
    PyObject *pyTitle = PyTuple_GetItem(pyTrackInfo, 3);
    PyObject *pyDuration = PyTuple_GetItem(pyTrackInfo, 4);

    quint32 trackNumber = PyLong_AsLong(pyTrackNumber);
    QString artist(PyUnicode_AsUTF8(pyArtist));
    QString album(PyUnicode_AsUTF8(pyAlbum));
    QString title(PyUnicode_AsUTF8(pyTitle));
    quint32 duration = PyLong_AsLong(pyDuration);

    QMediaMetaData metadata;
    metadata.insert(QMediaMetaData::Title, title);
    metadata.insert(QMediaMetaData::AlbumArtist, artist);
    metadata.insert(QMediaMetaData::AlbumTitle, album);
    metadata.insert(QMediaMetaData::TrackNumber, trackNumber);
    metadata.insert(QMediaMetaData::Duration, duration);
    metadata.insert(QMediaMetaData::AudioBitRate, 1411 * 1000);
    metadata.insert(QMediaMetaData::AudioCodec, 44100); // Using AudioCodec as sample rate for now

    emit this->durationChanged(duration);
    emit this->metadataChanged(metadata);

    Py_DECREF(pyTrackInfo);

    PyGILState_Release(state);

}

