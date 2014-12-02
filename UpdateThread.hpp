#ifndef UPDATETHREAD_HPP
#define UPDATETHREAD_HPP

#include <QThread>
#include <QSet>
#include <QPointF>

class UpdateThread : public QThread
{
        Q_OBJECT

    public:
        ~UpdateThread();
        static UpdateThread* instance();

    protected:
        void run();

    private:
        static UpdateThread* Instance;
        qint64 oldTimestamp;
        QSet<int> previousKeys;
        int previousMouseState;
        QPointF previousMousePos = QPointF(0, 0);
        QPointF previousMouseSpeed = QPointF(0, 0);
        QPointF previousMouseAcceleration = QPointF(0, 0);

        float speed = 10.f;
        float rotSpeed = 30.f;

    private:
        explicit UpdateThread();
        void update(float dT);

        // Méthodes utilisables dans l'update
        bool keyUp(int key);
        bool keyDown(int key);
        bool keyHeld(int key);
        bool mouseUp(int button);
        bool mouseDown(int button);
        bool mouseHeld(int button);
        QPointF mousePos();
        QPointF mouseSpeed(float dT);
        QPointF mouseAcceleration(float dT);
        QPointF mouseAccelerationDelta(float dT);
};

#endif // UPDATETHREAD_HPP
