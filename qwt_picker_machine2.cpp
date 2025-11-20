/******************************************************************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#include "qwt_picker_machine2.h"
#include "qwt_event_pattern.h"

#include <qevent.h>

//! Constructor
QwtPicker2Machine::QwtPicker2Machine( SelectionType type )
    : m_selectionType( type )
    , m_state( 0 )
{
}

//! Destructor
QwtPicker2Machine::~QwtPicker2Machine()
{
}

//! Return the selection type
QwtPicker2Machine::SelectionType QwtPicker2Machine::selectionType() const
{
    return m_selectionType;
}

//! Return the current state
int QwtPicker2Machine::state() const
{
    return m_state;
}

//! Change the current state
void QwtPicker2Machine::setState( int state )
{
    m_state = state;
}

//! Set the current state to 0.
void QwtPicker2Machine::reset()
{
    setState( 0 );
}

//! Constructor
QwtPicker2TrackerMachine::QwtPicker2TrackerMachine():
    QwtPicker2Machine( NoSelection )
{
}

//! Transition
QList< QwtPicker2Machine::Command > QwtPicker2TrackerMachine::transition(
    const QwtEventPattern&, const QEvent* e )
{
    QList< QwtPicker2Machine::Command > cmdList;

    switch ( e->type() )
    {
        case QEvent::Enter:
        case QEvent::MouseMove:
        {
            if ( state() == 0 )
            {
                cmdList += Begin;
                cmdList += Append;
                setState( 1 );
            }
            else
            {
                cmdList += Move;
            }
            break;
        }
        case QEvent::Leave:
        {
            cmdList += Remove;
            cmdList += End;
            setState( 0 );
        }
        default:
            break;
    }

    return cmdList;
}

//! Constructor
QwtPicker2ClickPointMachine::QwtPicker2ClickPointMachine():
    QwtPicker2Machine( PointSelection )
{
}

//! Transition
QList< QwtPicker2Machine::Command > QwtPicker2ClickPointMachine::transition(
    const QwtEventPattern& eventPattern, const QEvent* event )
{
    QList< QwtPicker2Machine::Command > cmdList;

    switch ( event->type() )
    {
        case QEvent::MouseButtonPress:
        {
            if ( eventPattern.mouseMatch( QwtEventPattern::MouseSelect2,
                static_cast< const QMouseEvent* >( event ) ) )
            {
                cmdList += Begin;
                cmdList += Append;
                cmdList += End;
            }
            break;
        }
        case QEvent::KeyPress:
        {
            const QKeyEvent* keyEvent = static_cast< const QKeyEvent* > ( event );
            if ( eventPattern.keyMatch( QwtEventPattern::KeySelect1, keyEvent ) )
            {
                if ( !keyEvent->isAutoRepeat() )
                {
                    cmdList += Begin;
                    cmdList += Append;
                    cmdList += End;
                }
            }
            break;
        }
        default:
            break;
    }

    return cmdList;
}

//! Constructor
QwtPicker2DragPointMachine::QwtPicker2DragPointMachine():
    QwtPicker2Machine( PointSelection )
{
}

//! Transition
QList< QwtPicker2Machine::Command > QwtPicker2DragPointMachine::transition(
    const QwtEventPattern& eventPattern, const QEvent* event )
{
    QList< QwtPicker2Machine::Command > cmdList;

    switch ( event->type() )
    {
        case QEvent::MouseButtonPress:
        {
            if ( eventPattern.mouseMatch( QwtEventPattern::MouseSelect2,
                static_cast< const QMouseEvent* >( event ) ) )
            {
                if ( state() == 0 )
                {
                    cmdList += Begin;
                    cmdList += Append;
                    setState( 1 );
                }
            }
            break;
        }
        case QEvent::MouseMove:
        case QEvent::Wheel:
        {
            if ( state() != 0 )
                cmdList += Move;
            break;
        }
        case QEvent::MouseButtonRelease:
        {
            if ( state() != 0 )
            {
                cmdList += End;
                setState( 0 );
            }
            break;
        }
        case QEvent::KeyPress:
        {
            const QKeyEvent* keyEvent = static_cast< const QKeyEvent* > ( event );
            if ( eventPattern.keyMatch( QwtEventPattern::KeySelect1, keyEvent ) )
            {
                if ( !keyEvent->isAutoRepeat() )
                {
                    if ( state() == 0 )
                    {
                        cmdList += Begin;
                        cmdList += Append;
                        setState( 1 );
                    }
                    else
                    {
                        cmdList += End;
                        setState( 0 );
                    }
                }
            }
            break;
        }
        default:
            break;
    }

    return cmdList;
}

//! Constructor
QwtPicker2ClickRectMachine::QwtPicker2ClickRectMachine():
    QwtPicker2Machine( RectSelection )
{
}

//! Transition
QList< QwtPicker2Machine::Command > QwtPicker2ClickRectMachine::transition(
    const QwtEventPattern& eventPattern, const QEvent* event )
{
    QList< QwtPicker2Machine::Command > cmdList;

    switch ( event->type() )
    {
        case QEvent::MouseButtonPress:
        {
            if ( eventPattern.mouseMatch( QwtEventPattern::MouseSelect2,
                static_cast< const QMouseEvent* >( event ) ) )
            {
                switch ( state() )
                {
                    case 0:
                    {
                        cmdList += Begin;
                        cmdList += Append;
                        setState( 1 );
                        break;
                    }
                    case 1:
                    {
                        // Uh, strange we missed the MouseButtonRelease
                        break;
                    }
                    default:
                    {
                        cmdList += End;
                        setState( 0 );
                    }
                }
            }
            break;
        }
        case QEvent::MouseMove:
        case QEvent::Wheel:
        {
            if ( state() != 0 )
                cmdList += Move;
            break;
        }
        case QEvent::MouseButtonRelease:
        {
            if ( eventPattern.mouseMatch( QwtEventPattern::MouseSelect2,
                static_cast< const QMouseEvent* >( event ) ) )
            {
                if ( state() == 1 )
                {
                    cmdList += Append;
                    setState( 2 );
                }
            }
            break;
        }
        case QEvent::KeyPress:
        {
            const QKeyEvent* keyEvent = static_cast< const QKeyEvent* > ( event );
            if ( eventPattern.keyMatch( QwtEventPattern::KeySelect1, keyEvent ) )
            {
                if ( !keyEvent->isAutoRepeat() )
                {
                    if ( state() == 0 )
                    {
                        cmdList += Begin;
                        cmdList += Append;
                        setState( 1 );
                    }
                    else
                    {
                        if ( state() == 1 )
                        {
                            cmdList += Append;
                            setState( 2 );
                        }
                        else if ( state() == 2 )
                        {
                            cmdList += End;
                            setState( 0 );
                        }
                    }
                }
            }
            break;
        }
        default:
            break;
    }

    return cmdList;
}

//! Constructor
QwtPicker2DragRectMachine::QwtPicker2DragRectMachine():
    QwtPicker2Machine( RectSelection )
{
}

//! Transition
QList< QwtPicker2Machine::Command > QwtPicker2DragRectMachine::transition(
    const QwtEventPattern& eventPattern, const QEvent* event )
{
    QList< QwtPicker2Machine::Command > cmdList;

    switch ( event->type() )
    {
        case QEvent::MouseButtonPress:
        {
            if ( eventPattern.mouseMatch( QwtEventPattern::MouseSelect2,
                static_cast< const QMouseEvent* >( event ) ) )
            {
                if ( state() == 0 )
                {
                    cmdList += Begin;
                    cmdList += Append;
                    cmdList += Append;
                    setState( 2 );
                }
            }
            break;
        }
        case QEvent::MouseMove:
        case QEvent::Wheel:
        {
            if ( state() != 0 )
                cmdList += Move;
            break;
        }
        case QEvent::MouseButtonRelease:
        {
            if ( state() == 2 )
            {
                cmdList += End;
                setState( 0 );
            }
            break;
        }
        case QEvent::KeyPress:
        {
            if ( eventPattern.keyMatch( QwtEventPattern::KeySelect1,
                static_cast< const QKeyEvent* > ( event ) ) )
            {
                if ( state() == 0 )
                {
                    cmdList += Begin;
                    cmdList += Append;
                    cmdList += Append;
                    setState( 2 );
                }
                else
                {
                    cmdList += End;
                    setState( 0 );
                }
            }
            break;
        }
        default:
            break;
    }

    return cmdList;
}

//! Constructor
QwtPicker2PolygonMachine::QwtPicker2PolygonMachine():
    QwtPicker2Machine( PolygonSelection )
{
}

//! Transition
QList< QwtPicker2Machine::Command > QwtPicker2PolygonMachine::transition(
    const QwtEventPattern& eventPattern, const QEvent* event )
{
    QList< QwtPicker2Machine::Command > cmdList;

    switch ( event->type() )
    {
        case QEvent::MouseButtonPress:
        {
            if ( eventPattern.mouseMatch( QwtEventPattern::MouseSelect2,
                static_cast< const QMouseEvent* >( event ) ) )
            {
                if ( state() == 0 )
                {
                    cmdList += Begin;
                    cmdList += Append;
                    cmdList += Append;
                    setState( 1 );
                }
                else
                {
                    cmdList += Append;
                }
            }
            if ( eventPattern.mouseMatch( QwtEventPattern::MouseSelect2,
                static_cast< const QMouseEvent* >( event ) ) )
            {
                if ( state() == 1 )
                {
                    cmdList += End;
                    setState( 0 );
                }
            }
            break;
        }
        case QEvent::MouseMove:
        case QEvent::Wheel:
        {
            if ( state() != 0 )
                cmdList += Move;
            break;
        }
        case QEvent::KeyPress:
        {
            const QKeyEvent* keyEvent = static_cast< const QKeyEvent* > ( event );
            if ( eventPattern.keyMatch( QwtEventPattern::KeySelect1, keyEvent ) )
            {
                if ( !keyEvent->isAutoRepeat() )
                {
                    if ( state() == 0 )
                    {
                        cmdList += Begin;
                        cmdList += Append;
                        cmdList += Append;
                        setState( 1 );
                    }
                    else
                    {
                        cmdList += Append;
                    }
                }
            }
            else if ( eventPattern.keyMatch( QwtEventPattern::KeySelect2, keyEvent ) )
            {
                if ( !keyEvent->isAutoRepeat() )
                {
                    if ( state() == 1 )
                    {
                        cmdList += End;
                        setState( 0 );
                    }
                }
            }
            break;
        }
        default:
            break;
    }

    return cmdList;
}

//! Constructor
QwtPicker2DragLineMachine::QwtPicker2DragLineMachine():
    QwtPicker2Machine( PolygonSelection )
{
}

//! Transition
QList< QwtPicker2Machine::Command > QwtPicker2DragLineMachine::transition(
    const QwtEventPattern& eventPattern, const QEvent* event )
{
    QList< QwtPicker2Machine::Command > cmdList;

    switch( event->type() )
    {
        case QEvent::MouseButtonPress:
        {
            if ( eventPattern.mouseMatch( QwtEventPattern::MouseSelect2,
                static_cast< const QMouseEvent* >( event ) ) )
            {
                if ( state() == 0 )
                {
                    cmdList += Begin;
                    cmdList += Append;
                    cmdList += Append;
                    setState( 1 );
                }
            }
            break;
        }
        case QEvent::KeyPress:
        {
            if ( eventPattern.keyMatch( QwtEventPattern::KeySelect1,
                static_cast< const QKeyEvent* > ( event ) ) )
            {
                if ( state() == 0 )
                {
                    cmdList += Begin;
                    cmdList += Append;
                    cmdList += Append;
                    setState( 1 );
                }
                else
                {
                    cmdList += End;
                    setState( 0 );
                }
            }
            break;
        }
        case QEvent::MouseMove:
        case QEvent::Wheel:
        {
            if ( state() != 0 )
                cmdList += Move;

            break;
        }
        case QEvent::MouseButtonRelease:
        {
            if ( state() != 0 )
            {
                cmdList += End;
                setState( 0 );
            }
        }
        default:
            break;
    }

    return cmdList;
}
