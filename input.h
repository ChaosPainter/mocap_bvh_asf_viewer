#ifndef INPUT_H
#define INPUT_H

#include <Qt>
#include <QPoint>

class Input
{
public:

  enum InputState
  {
    InputInvalid,
    InputRegistered,
    InputUnregistered,
    InputTriggered,
    InputPressed,
    InputReleased
  };


  static InputState keyState(Qt::Key key);
  static bool keyPressed(Qt::Key key);
  static InputState buttonState(Qt::MouseButton button);
  static bool buttonPressed(Qt::MouseButton button);
  static QPoint mouseDelta();

private:


  static void update();
  static void registerKeyPress(int key);
  static void registerKeyRelease(int key);
  static void registerMousePress(Qt::MouseButton button);
  static void registerMouseRelease(Qt::MouseButton button);

  friend class Qwidgetgl;
};


inline bool Input::keyPressed(Qt::Key key)
{
  return keyState(key) == InputPressed;
}


inline bool Input::buttonPressed(Qt::MouseButton button)
{
  return buttonState(button) == InputPressed;
}

#endif // INPUT_H
