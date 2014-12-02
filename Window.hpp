#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <QGLWidget>
#include <QVector>
#include <QMutex>
#include <QSet>
#include <QApplication>
#include "OBJLoader.hpp"

#define wi Window::instance()

class Window : public QGLWidget
{
        Q_OBJECT

    friend class UpdateThread;

    public:
        ~Window();
        static Window* instance();
        QMutex* mutex();

    private:
        Window(QWidget *parent = 0);
        static Window* Instance;
        QMutex *Mutex = nullptr;

        // État actuel
        QSet<int> KeysPressed;
        int MButtonsPressed = Qt::NoButton;
        QPointF mousePos;

    protected:
        void initializeGL();
        void resizeGL(int w, int h);
        void paintGL();
        void keyPressEvent(QKeyEvent *e);
        void keyReleaseEvent(QKeyEvent *e);
        void mousePressEvent(QMouseEvent *e);
        void mouseReleaseEvent(QMouseEvent *e);
        void mouseMoveEvent(QMouseEvent *e);

    private:
        QPointF toNormCoords(QPoint pos);
        QPoint fromNormCoords(QPointF pos);
};

#endif // WINDOW_HPP

