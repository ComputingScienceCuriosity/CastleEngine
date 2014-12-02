#include "UpdateThread.hpp"
#include <QDateTime>
#include <QVector3D>
#include <QtMath>
#include "Window.hpp"
#include "Renderer.hpp"

#define PI 3.14159265f
#define PI_OVER_180 PI / 180.f
#define V_FORWARD   QVector3D(0.f, 0.f, -1.f);
#define V_BACK      QVector3D(0.f, 0.f, +1.f);
#define V_LEFT      QVector3D(-1.f, 0.f, 0.f);
#define V_RIGHT     QVector3D(+1.f, 0.f, 0.f);
#define V_UP        QVector3D(0.f, +1.f, 0.f);
#define V_DOWN      QVector3D(0.f, -1.f, 0.f);

UpdateThread* UpdateThread::Instance = nullptr;

UpdateThread::UpdateThread() :
    QThread(nullptr)
{
    oldTimestamp = QDateTime::currentMSecsSinceEpoch();
}

UpdateThread::~UpdateThread()
{
    Instance = nullptr;
}

UpdateThread*UpdateThread::instance()
{
    if (Instance == nullptr)
        Instance = new UpdateThread;
    return Instance;
}

void UpdateThread::run()
{
    // Fais tourner l'update en infini
    forever
    {
        // Gestion du temps
        qint64 newTimestamp = QDateTime::currentMSecsSinceEpoch();
        long double dT = ((long double)(newTimestamp) - (long double)(oldTimestamp)) / 1000.f;
        oldTimestamp = newTimestamp;

        // Appel de l'update
        update(dT);
    }
}

void UpdateThread::update(float dT)
{
    // Prends le mutex
    wi->mutex()->lock();

    /*---------------------------------------*/
    /* DÉBUT DU CODE UTILISATEUR DE L'UPDATE */
    /*---------------------------------------*/


    // On quitte dès que la touche Échap est pressée
    if (keyDown(Qt::Key_Escape))
        qApp->exit(0);

    // Mouse tracking toggler
    if (mouseDown(Qt::MiddleButton))
        wi->setMouseTracking(!wi->hasMouseTracking());

    // Switching shaders
    if (keyDown(Qt::Key_F2))
        rdi->nextShaderProgram();

    // Next Castle
    if (keyDown(Qt::Key_G))
    {
        qsrand((uint) QDateTime::currentMSecsSinceEpoch());
        int randnum = qrand() % 9;
        rdi->nextCastle((randnum%3) + 1,(randnum/3) + 1);
        wi->update();
    }


    // Déplacement
    if (keyHeld(Qt::Key_Z))
    {
        rdi->Position += (float)qSin(rdi->Heading*PI_OVER_180) * speed * dT * V_LEFT;
        rdi->Position += (float)qCos(rdi->Heading*PI_OVER_180) * speed * dT * V_FORWARD;
        rdi->Position += (float)qSin(rdi->LookUpDown*PI_OVER_180) * speed * dT * V_UP;
        wi->update();
    }
    if (keyHeld(Qt::Key_S))
    {
        rdi->Position += (float)qSin(rdi->Heading*PI_OVER_180) * speed * dT * V_RIGHT;
        rdi->Position += (float)qCos(rdi->Heading*PI_OVER_180) * speed * dT * V_BACK;
        rdi->Position += (float)qSin(rdi->LookUpDown*PI_OVER_180) * speed * dT * V_DOWN;
        wi->update();
    }
    if (keyHeld(Qt::Key_Q))
    {
        rdi->Position += (float)qSin(rdi->Heading*PI_OVER_180) * speed * dT * V_BACK;
        rdi->Position += (float)qCos(rdi->Heading*PI_OVER_180) * speed * dT * V_LEFT;
        wi->update();
    }
    if (keyHeld(Qt::Key_D))
    {
        rdi->Position += (float)qSin(rdi->Heading*PI_OVER_180) * speed * dT * V_FORWARD;
        rdi->Position += (float)qCos(rdi->Heading*PI_OVER_180) * speed * dT * V_RIGHT;
        wi->update();
    }
    if (keyHeld(Qt::Key_Space))
    {
        rdi->Position += speed * dT * V_UP;
        wi->update();
    }
    if (keyHeld(Qt::Key_Shift))
    {
        rdi->Position += speed * dT * V_DOWN;
        wi->update();
    }
    if (keyHeld(Qt::Key_Up))
    {
        rdi->LookUpDown -= 1.0f * dT * rotSpeed;
        wi->update();
    }
    if (keyHeld(Qt::Key_Down))
    {
        rdi->LookUpDown += 1.0f * dT * rotSpeed;
        wi->update();
    }
    if (keyHeld(Qt::Key_Left))
    {
        rdi->Heading += 1.0f * dT * rotSpeed;
        wi->update();
    }
    if (keyHeld(Qt::Key_Right))
    {
        rdi->Heading -= 1.0f * dT * rotSpeed;
        wi->update();
    }


    /*-------------------------------------*/
    /* FIN DU CODE UTILISATEUR DE L'UPDATE */
    /*-------------------------------------*/

    // On essaie de faire en sorte que le curseur reste DANS la fenêtre lorsqu'on se déplace
    QPointF nextPos = mousePos() + mouseSpeed(dT) * dT/* + mouseAcceleration(dT) * dT * dT*/;
    if (nextPos.x() < -1 || nextPos.x() > 1 || nextPos.y() < -1 || nextPos.y() > 1)
    {
        if (nextPos.x() < -1.) nextPos.rx() += 2.;
        if (nextPos.x() > 1.) nextPos.rx() -= 2.;
        if (nextPos.y() < -1.) nextPos.ry() += 2.;
        if (nextPos.y() > 1.) nextPos.ry() -= 2.;

        QCursor::setPos(wi->fromNormCoords(nextPos));
    }

    // Mise à jour des entrées
    previousKeys = wi->KeysPressed;
    previousMouseState = wi->MButtonsPressed;
    QPointF newMousePos = mousePos();
    QPointF newMouseSpeed = mouseSpeed(dT);
    QPointF newMouseAcceleration = mouseAcceleration(dT);
    previousMousePos = newMousePos;
    previousMouseSpeed = newMouseSpeed;
    previousMouseAcceleration = newMouseAcceleration;

    // Libère le mutex
    wi->mutex()->unlock();

    // Délai minimum entre les updates pour ne pas mobiliser trop de processeur
    QThread::msleep(10);
}

bool UpdateThread::keyUp(int key)
{
    return (previousKeys.contains(key) && !wi->KeysPressed.contains(key));
}

bool UpdateThread::keyDown(int key)
{
    return (!previousKeys.contains(key) && wi->KeysPressed.contains(key));
}

bool UpdateThread::keyHeld(int key)
{
    return wi->KeysPressed.contains(key);
}

bool UpdateThread::mouseUp(int button)
{
    return (previousMouseState & button) && !(wi->MButtonsPressed & button);
}

bool UpdateThread::mouseDown(int button)
{
    return !(previousMouseState & button) && (wi->MButtonsPressed & button);
}

bool UpdateThread::mouseHeld(int button)
{
    return (wi->MButtonsPressed & button);
}

QPointF UpdateThread::mousePos()
{
    return wi->mousePos;
}

QPointF UpdateThread::mouseSpeed(float dT)
{
    return (mousePos() - previousMousePos) / dT;
}

QPointF UpdateThread::mouseAcceleration(float dT)
{
    return (mouseSpeed(dT) - previousMouseSpeed) / dT;
}

QPointF UpdateThread::mouseAccelerationDelta(float dT)
{
    return (mouseAcceleration(dT) - previousMouseAcceleration) / dT;
}
