/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_PICKER_MACHINE2
#define QWT_PICKER_MACHINE2 1

#include "qwt_global.h"
#include <qlist.h>

class QEvent;
class QwtEventPattern;

/*!
  \brief A state machine for QwtPicker selections

  QwtPickerMachine accepts key and mouse events and translates them
  into selection commands.

  \sa QwtEventPattern::MousePatternCode, QwtEventPattern::KeyPatternCode
*/

class QWT_EXPORT QwtPicker2Machine
{
public:
    /*!
      Type of a selection.
      \sa selectionType()
    */
    enum SelectionType
    {
        //! The state machine not usable for any type of selection.
        NoSelection = -1,

        //! The state machine is for selecting a single point.
        PointSelection,

        //! The state machine is for selecting a rectangle (2 points).
        RectSelection,

        //! The state machine is for selecting a polygon (many points).
        PolygonSelection
    };

    //! Commands - the output of a state machine
    enum Command
    {
        Begin,
        Append,
        Move,
        Remove,
        End
    };

    QwtPicker2Machine( SelectionType );
    virtual ~QwtPicker2Machine();

    //! Transition
    virtual QList<Command> transition(
        const QwtEventPattern &, const QEvent * ) = 0;
    void reset();

    int state() const;
    void setState( int );

    SelectionType selectionType() const;

private:
    const SelectionType d_selectionType;
    int d_state;
};

/*!
  \brief A state machine for indicating mouse movements

  QwtPickerTrackerMachine supports displaying information
  corresponding to mouse movements, but is not intended for
  selecting anything. Begin/End are related to Enter/Leave events.
*/
class QWT_EXPORT QwtPicker2TrackerMachine: public QwtPicker2Machine
{
public:
    QwtPicker2TrackerMachine();

    virtual QList<Command> transition(
        const QwtEventPattern &, const QEvent * );
};

/*!
  \brief A state machine for point selections

  Pressing QwtEventPattern::MouseSelect1 or
  QwtEventPattern::KeySelect1 selects a point.

  \sa QwtEventPattern::MousePatternCode, QwtEventPattern::KeyPatternCode
*/
class QWT_EXPORT QwtPicker2ClickPointMachine: public QwtPicker2Machine
{
public:
    QwtPicker2ClickPointMachine();

    virtual QList<Command> transition(
        const QwtEventPattern &, const QEvent * );
};

/*!
  \brief A state machine for point selections

  Pressing QwtEventPattern::MouseSelect1 or QwtEventPattern::KeySelect1
  starts the selection, releasing QwtEventPattern::MouseSelect1 or
  a second press of QwtEventPattern::KeySelect1 terminates it.
*/
class QWT_EXPORT QwtPicker2DragPointMachine: public QwtPicker2Machine
{
public:
    QwtPicker2DragPointMachine();

    virtual QList<Command> transition(
        const QwtEventPattern &, const QEvent * );
};

/*!
  \brief A state machine for rectangle selections

  Pressing QwtEventPattern::MouseSelect1 starts
  the selection, releasing it selects the first point. Pressing it
  again selects the second point and terminates the selection.
  Pressing QwtEventPattern::KeySelect1 also starts the
  selection, a second press selects the first point. A third one selects
  the second point and terminates the selection.

  \sa QwtEventPattern::MousePatternCode, QwtEventPattern::KeyPatternCode
*/

class QWT_EXPORT QwtPicker2ClickRectMachine: public QwtPicker2Machine
{
public:
    QwtPicker2ClickRectMachine();

    virtual QList<Command> transition(
        const QwtEventPattern &, const QEvent * );
};

/*!
  \brief A state machine for rectangle selections

  Pressing QwtEventPattern::MouseSelect1 selects
  the first point, releasing it the second point.
  Pressing QwtEventPattern::KeySelect1 also selects the
  first point, a second press selects the second point and terminates
  the selection.

  \sa QwtEventPattern::MousePatternCode, QwtEventPattern::KeyPatternCode
*/

class QWT_EXPORT QwtPicker2DragRectMachine: public QwtPicker2Machine
{
public:
    QwtPicker2DragRectMachine();

    virtual QList<Command> transition(
        const QwtEventPattern &, const QEvent * );
};

/*!
  \brief A state machine for line selections
    
  Pressing QwtEventPattern::MouseSelect1 selects
  the first point, releasing it the second point.
  Pressing QwtEventPattern::KeySelect1 also selects the
  first point, a second press selects the second point and terminates
  the selection.

  A common use case of QwtPickerDragLineMachine are pickers for
  distance measurements.

  \sa QwtEventPattern::MousePatternCode, QwtEventPattern::KeyPatternCode
*/

class QWT_EXPORT QwtPicker2DragLineMachine: public QwtPicker2Machine
{
public:
    QwtPicker2DragLineMachine();

    virtual QList<Command> transition(
        const QwtEventPattern &, const QEvent * );
};

/*!
  \brief A state machine for polygon selections

  Pressing QwtEventPattern::MouseSelect1 or QwtEventPattern::KeySelect1
  starts the selection and selects the first point, or appends a point.
  Pressing QwtEventPattern::MouseSelect2 or QwtEventPattern::KeySelect2
  appends the last point and terminates the selection.

  \sa QwtEventPattern::MousePatternCode, QwtEventPattern::KeyPatternCode
*/

class QWT_EXPORT QwtPicker2PolygonMachine: public QwtPicker2Machine
{
public:
    QwtPicker2PolygonMachine();

    virtual QList<Command> transition(
        const QwtEventPattern &, const QEvent * );
};

#endif
