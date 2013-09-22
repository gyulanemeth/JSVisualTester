#ifndef COMMANDS_HPP
#define COMMANDS_HPP

#include <string>

#define STRLENGTH

class Command
{
public:
	Command ();
	virtual ~Command ();

	virtual void Execute () = 0;
};

class SleepCommand : public Command
{
public:
	SleepCommand (int milliSeconds);
	virtual ~SleepCommand ();

	virtual void Execute ();

protected:
	int milliSeconds;
};

class MoveWindowCommand : public Command
{
public:
	MoveWindowCommand (const RECT& rect);
	virtual ~MoveWindowCommand ();

	virtual void Execute ();

protected:
	RECT rect;
};

class KeyPressCommand : public Command
{
public:
	KeyPressCommand (byte keyCode, byte specialKeyCode);
	virtual ~KeyPressCommand ();

	virtual void Execute ();

protected:
	byte keyCode;
	byte specialKeyCode;
};

class TextWriteCommand : public Command
{
public:
	TextWriteCommand (const std::wstring& text);
	virtual ~TextWriteCommand ();

	virtual void Execute ();

protected:
	std::wstring text;
};

class MouseCommand : public Command
{
public:
	MouseCommand (int xCoord, int yCoord);
	virtual ~MouseCommand ();

	virtual void Execute () = 0;

protected:
	int xCoord;
	int yCoord;
};

class MouseClickCommand : public MouseCommand
{
public:
	MouseClickCommand (int xCoord, int yCoord);
	~MouseClickCommand ();

	virtual void Execute ();
};

class MouseDoubleClickCommand : public MouseCommand
{
public:
	MouseDoubleClickCommand (int xCoord, int yCoord);
	~MouseDoubleClickCommand ();

	virtual void Execute ();
};

class MouseDragDropCommand : public MouseCommand
{
public:
	MouseDragDropCommand (int xCoord, int yCoord, int newXCoord, int newYCoord, int milliSeconds);
	~MouseDragDropCommand ();

	virtual void Execute ();

private:
	int newXCoord;
	int newYCoord;
	int milliSeconds;
};

#endif
