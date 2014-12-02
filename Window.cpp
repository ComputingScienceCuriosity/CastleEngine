#include "Window.hpp"
#include <GL/glu.h>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QMessageBox>
#include "UpdateThread.hpp"
#include "Renderer.hpp"

Window* Window::Instance = nullptr;

Window::Window(QWidget *parent)
    : QGLWidget(parent)
{
}

Window::~Window()
{
    Window::Instance = nullptr;
}

Window* Window::instance()
{
    if (Window::Instance == nullptr)
        Window::Instance = new Window;
    return Window::Instance;
}

QMutex*Window::mutex()
{
    // Si le mutex n'existe pas, le crée
    if (Mutex == nullptr)
        Mutex = new QMutex;
    // Renvoie le mutex
    return Mutex;
}

void Window::initializeGL()
{
    rdi->load();

    // Lancement du thread d'update
    UpdateThread::instance()->start();
}

void Window::resizeGL(int w, int h)
{
    rdi->aspectRatio(w, h);
}

void Window::paintGL()
{
    mutex()->lock();

    rdi->render();

    mutex()->unlock();
}

void Window::keyPressEvent(QKeyEvent *e)
{
    // On ne prends pas en compte les répétitions automatiques
    if (e->isAutoRepeat())
        return;
    // Ajoute la touche comme pressée
    mutex()->lock();
    KeysPressed.insert(e->key());
    mutex()->unlock();
}

void Window::keyReleaseEvent(QKeyEvent *e)
{
    // On ne prends pas en compte les répétitions automatiques
    if (e->isAutoRepeat())
        return;
    // Enlève la touche des touches pressées
    mutex()->lock();
    KeysPressed.remove(e->key());
    mutex()->unlock();
}

void Window::mousePressEvent(QMouseEvent* e)
{
    // Ajoute le bouton pressé dans l'état de la souris
    mutex()->lock();
    MButtonsPressed |= e->button();
    mutex()->unlock();
}

void Window::mouseReleaseEvent(QMouseEvent* e)
{
    // Retirer le bouton relâché de l'état de la souris
    mutex()->lock();
    MButtonsPressed &= (e->button() ^ -1);
    mutex()->unlock();
}

void Window::mouseMoveEvent(QMouseEvent* e)
{
    // Tracer le déplacement de la souris
    mutex()->lock();
    mousePos = toNormCoords(e->globalPos());
    mutex()->unlock();
}

QPointF Window::toNormCoords(QPoint pos)
{
    qreal xpos = (2 * qreal(pos.x() - geometry().x())) / (geometry().width() - 1) - 1;
    qreal ypos = (-2 * qreal(pos.y() - geometry().y())) / (geometry().height() - 1) + 1;
    return QPointF(xpos, ypos);
}

QPoint Window::fromNormCoords(QPointF pos)
{
    qreal xpos = ((pos.x() + 1) * geometry().width()) / 2. + geometry().x();
    qreal ypos = (((-pos.y()) + 1) * geometry().height()) / 2. + geometry().y();
    return QPoint(xpos, ypos);
}
