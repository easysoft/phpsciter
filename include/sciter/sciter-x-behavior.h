/*
 * The Sciter Engine of Terra Informatica Software, Inc.
 * http://sciter.com
 * 
 * The code and information provided "as-is" without
 * warranty of any kind, either expressed or implied.
 * 
 * (C) 2003-2015, Terra Informatica Software, Inc.
 */

/*
 * Behaviors support (a.k.a windowless controls)
 */

#ifndef __sciter_x_behavior_h__
#define __sciter_x_behavior_h__

/*!\file
\brief Behaiviors support (a.k.a windowless scriptable controls)
*/

#include "sciter-x-types.h"
#include "sciter-x-dom.h"
#include "sciter-x-value.h"
#include "sciter-x-graphics.h"
#include "sciter-om.h"

#pragma pack(push,8)

  /** event groups.
       **/
  typedef enum EVENT_GROUPS
  {
      HANDLE_INITIALIZATION = 0x0000, /**< attached/detached */
      HANDLE_MOUSE = 0x0001,          /**< mouse events */
      HANDLE_KEY = 0x0002,            /**< key events */
      HANDLE_FOCUS = 0x0004,          /**< focus events, if this flag is set it also means that element it attached to is focusable */
      HANDLE_SCROLL = 0x0008,         /**< scroll events */
      HANDLE_TIMER = 0x0010,          /**< timer event */
      HANDLE_SIZE = 0x0020,           /**< size changed event */
      HANDLE_DRAW = 0x0040,           /**< drawing request (event) */
      HANDLE_DATA_ARRIVED = 0x080,    /**< requested data () has been delivered */
      HANDLE_BEHAVIOR_EVENT        = 0x0100, /**< logical, synthetic events:
                                                 BUTTON_CLICK, HYPERLINK_CLICK, etc.,
                                                 a.k.a. notifications from intrinsic behaviors */
      HANDLE_METHOD_CALL           = 0x0200, /**< behavior specific methods */
      HANDLE_SCRIPTING_METHOD_CALL = 0x0400, /**< behavior specific methods */
      //HANDLE_TISCRIPT_METHOD_CALL  = 0x0800, /**< behavior specific methods using direct tiscript::value's */

      HANDLE_EXCHANGE              = 0x1000, /**< system drag-n-drop */
      HANDLE_GESTURE               = 0x2000, /**< touch input events */

      HANDLE_SOM                   = 0x8000, /**< som_asset_t request */

      HANDLE_ALL                   = 0xFFFF, /*< all of them */

      SUBSCRIPTIONS_REQUEST        = 0xFFFFFFFF, /**< special value for getting subscription flags */
  } EVENT_GROUPS;

/**Element callback function for all types of events. Similar to WndProc
 * \param tag \b LPVOID, tag assigned by SciterAttachEventHandler function (like GWL_USERDATA)
 * \param he \b HELEMENT, this element handle (like HWINDOW)
 * \param evtg \b UINT, group identifier of the event, value is one of EVENT_GROUPS
 * \param prms \b LPVOID, pointer to group specific parameters structure.
 * \return true if event was handled, false otherwise.
 **/

typedef  SBOOL SC_CALLBACK ElementEventProc(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms );
typedef  ElementEventProc * LPElementEventProc;
// signature of the function exported from external behavior/dll.
typedef SBOOL SC_CALLBACK SciterBehaviorFactory( LPCSTR, HELEMENT, LPElementEventProc*, LPVOID* );

  typedef enum PHASE_MASK
  {
      BUBBLING = 0,      // bubbling (emersion) phase
      SINKING  = 0x8000,  // capture (immersion) phase, this flag is or'ed with EVENTS codes below
      HANDLED  = 0x10000
    // see: http://www.w3.org/TR/xml-events/Overview.html#s_intro
  } PHASE_MASK;

  typedef enum MOUSE_BUTTONS
  {
      MAIN_MOUSE_BUTTON = 1, //aka left button
      PROP_MOUSE_BUTTON = 2, //aka right button
      MIDDLE_MOUSE_BUTTON = 4,
  } MOUSE_BUTTONS;

  typedef enum KEYBOARD_STATES
  {
      CONTROL_KEY_PRESSED = 0x1,
      SHIFT_KEY_PRESSED = 0x2,
      ALT_KEY_PRESSED = 0x4
  } KEYBOARD_STATES;

// parameters of evtg == HANDLE_INITIALIZATION

  typedef enum INITIALIZATION_EVENTS
  {
    BEHAVIOR_DETACH = 0,
    BEHAVIOR_ATTACH = 1
  } INITIALIZATION_EVENTS;

  typedef struct INITIALIZATION_PARAMS
  {
    UINT cmd; // INITIALIZATION_EVENTS
  } INITIALIZATION_PARAMS;

  typedef enum SOM_EVENTS
  {
    SOM_GET_PASSPORT = 0,
    SOM_GET_ASSET = 1
  } SOM_EVENTS;

  typedef struct SOM_PARAMS
  {
    UINT cmd; // SOM_EVENTS
    union {
      som_passport_t* passport;
      som_asset_t*    asset;
    } data;
#ifdef __cplusplus
    SOM_PARAMS() : data() {}
#endif
  } SOM_PARAMS;

  typedef enum DRAGGING_TYPE
  {
    NO_DRAGGING,
    DRAGGING_MOVE,
    DRAGGING_COPY,
  } DRAGGING_TYPE;

// parameters of evtg == HANDLE_MOUSE

  typedef enum MOUSE_EVENTS
  {
      MOUSE_ENTER = 0,
      MOUSE_LEAVE,
      MOUSE_MOVE,
      MOUSE_UP,
      MOUSE_DOWN,
      MOUSE_DCLICK, // double click
      MOUSE_WHEEL,
      MOUSE_TICK, // mouse pressed ticks
      MOUSE_IDLE, // mouse stay idle for some time

      /*OBSOLETE*/ DROP        = 9,   // item dropped, target is that dropped item 
      /*OBSOLETE*/ DRAG_ENTER  = 0xA, // drag arrived to the target element that is one of current drop targets.  
      /*OBSOLETE*/ DRAG_LEAVE  = 0xB, // drag left one of current drop targets. target is the drop target element.  
      /*OBSOLETE*/ DRAG_REQUEST = 0xC,  // drag src notification before drag start. To cancel - return true from handler.

      MOUSE_TCLICK = 0xF, // tripple click

      MOUSE_TOUCH_START = 0xFC, // touch device pressed somehow
      MOUSE_TOUCH_END = 0xFD,   // touch device depressed - clear, nothing on it

      MOUSE_DRAG_REQUEST = 0xFE, // mouse drag start detected event

      MOUSE_CLICK = 0xFF, // mouse click event

      /*OBSOLETE*/ DRAGGING = 0x100, // ORed with MOUSE_ENTER...MOUSE_DOWN codes above

      MOUSE_HIT_TEST = 0xFFE,    // sent to element, allows to handle elements with non-trivial shapes. 

  } MOUSE_EVENTS;

  typedef struct MOUSE_PARAMS
  {
      UINT      cmd;          // MOUSE_EVENTS
      HELEMENT  target;       // target element
      POINT     pos;          // position of cursor, element relative
      POINT     pos_view;     // position of cursor, view relative
      UINT      button_state; // MOUSE_BUTTONS
      UINT      alt_state;    // KEYBOARD_STATES
      UINT      cursor_type;  // CURSOR_TYPE to set, see CURSOR_TYPE
      SBOOL      is_on_icon;   // mouse is over icon (foreground-image, foreground-repeat:no-repeat)

      HELEMENT  dragging;     // element that is being dragged over, this field is not NULL if (cmd & DRAGGING) != 0
      UINT      dragging_mode;// see DRAGGING_TYPE. 

  } MOUSE_PARAMS;

  typedef enum CURSOR_TYPE
  {
      CURSOR_ARROW, //0
      CURSOR_IBEAM, //1
      CURSOR_WAIT,  //2
      CURSOR_CROSS, //3
      CURSOR_UPARROW,  //4
      CURSOR_SIZENWSE, //5
      CURSOR_SIZENESW, //6
      CURSOR_SIZEWE,   //7
      CURSOR_SIZENS,   //8
      CURSOR_SIZEALL,  //9
      CURSOR_NO,       //10
      CURSOR_APPSTARTING, //11
      CURSOR_HELP,        //12
      CURSOR_HAND,        //13
      CURSOR_DRAG_MOVE,   //14
      CURSOR_DRAG_COPY,   //15
  } CURSOR_TYPE;


// parameters of evtg == HANDLE_KEY

  typedef enum KEY_EVENTS
  {
      KEY_DOWN = 0,
      KEY_UP,
      KEY_CHAR
  } KEY_EVENTS;

  typedef struct KEY_PARAMS
  {
      UINT      cmd;          // KEY_EVENTS
      HELEMENT  target;       // target element
      UINT      key_code;     // key scan code, or character unicode for KEY_CHAR
      UINT      alt_state;    // KEYBOARD_STATES
  } KEY_PARAMS;


  /** #HANDLE_FOCUS commands */
  typedef enum FOCUS_EVENTS
  {
      FOCUS_OUT = 0,            /**< container lost focus from any element inside it, target is an element that lost focus */
      FOCUS_IN = 1,             /**< container got focus on element inside it, target is an element that got focus */
      FOCUS_GOT = 2,            /**< target element got focus */
      FOCUS_LOST = 3,           /**< target element lost focus */
      FOCUS_REQUEST = 4,        /**< bubbling event/request, gets sent on child-parent chain to accept/reject focus to be set on the child (target) */
      FOCUS_ADVANCE_REQUEST = 5,/**< bubbling event/request, gets sent on child-parent chain to advance focus */
  } FOCUS_EVENTS;

  typedef enum FOCUS_CMD_TYPE {
      FOCUS_RQ_NEXT,
      FOCUS_RQ_PREV,
      FOCUS_RQ_HOME,
      FOCUS_RQ_END,
      FOCUS_RQ_LEFT,
      FOCUS_RQ_RIGHT,
      FOCUS_RQ_UP,
      FOCUS_RQ_DOWN,  // all these - by key
      FOCUS_RQ_FIRST, // these two - by_code
      FOCUS_RQ_LAST,  //
      FOCUS_RQ_END_REACHED = 0x8000
  } FOCUS_CMD_TYPE;

  /** #HANDLE_FOCUS params */
  typedef struct FOCUS_PARAMS
  {
      UINT      cmd;            /**< #FOCUS_EVENTS */
      HELEMENT  target;         /**< target element, for #FOCUS_LOST it is a handle of new focus element
                                     and for #FOCUS_GOT it is a handle of old focus element, can be NULL */
      UINT      cause;          /**< focus cause params or FOCUS_CMD_TYPE for FOCUS_ADVANCE_REQUEST */
      SBOOL      cancel;         /**< in #FOCUS_REQUEST and #FOCUS_LOST phase setting this field to true will cancel transfer focus from old element to the new one. */
  } FOCUS_PARAMS;

// parameters of evtg == HANDLE_SCROLL

  typedef enum SCROLL_EVENTS
  {
    SCROLL_HOME = 0,
    SCROLL_END,
    SCROLL_STEP_PLUS,
    SCROLL_STEP_MINUS,
    SCROLL_PAGE_PLUS,
    SCROLL_PAGE_MINUS,
    SCROLL_POS,
    SCROLL_SLIDER_RELEASED,
    SCROLL_CORNER_PRESSED,
    SCROLL_CORNER_RELEASED,
    SCROLL_SLIDER_PRESSED,

    SCROLL_ANIMATION_START,
    SCROLL_ANIMATION_END,
  } SCROLL_EVENTS;

  typedef enum SCROLL_SOURCE {
    SCROLL_SOURCE_UNKNOWN,
    SCROLL_SOURCE_KEYBOARD,  // SCROLL_PARAMS::reason <- keyCode
    SCROLL_SOURCE_SCROLLBAR, // SCROLL_PARAMS::reason <- SCROLLBAR_PART 
    SCROLL_SOURCE_ANIMATOR,
    SCROLL_SOURCE_WHEEL,
  } SCROLL_SOURCE;

  typedef enum SCROLLBAR_PART {
    SCROLLBAR_BASE,       
    SCROLLBAR_PLUS,       
    SCROLLBAR_MINUS,      
    SCROLLBAR_SLIDER,     
    SCROLLBAR_PAGE_MINUS, 
    SCROLLBAR_PAGE_PLUS,  
    SCROLLBAR_CORNER,     
  } SCROLLBAR_PART;


  typedef struct SCROLL_PARAMS
  {
      UINT      cmd;          // SCROLL_EVENTS
      HELEMENT  target;       // target element
      INT       pos;          // scroll position if SCROLL_POS
      SBOOL      vertical;     // true if from vertical scrollbar
      UINT      source;       // SCROLL_SOURCE
      UINT      reason;       // key or scrollbar part
  } SCROLL_PARAMS;

  typedef enum GESTURE_CMD
  {
    GESTURE_START = 0,
    GESTURE_MOVE = 1,
    GESTURE_END = 2,

    // logical events
    GESTURE_PAN,    // The pan gesture.
    GESTURE_ZOOM,        // The zoom gesture.
    GESTURE_ROTATE,      // The rotation gesture.
    GESTURE_TAP1,   // The tap gesture, a.k.a. click
    GESTURE_TAP2,   // The two-finger tap gesture, a.k.a. right-click
    GESTURE_DOUBLE_TAP
  } GESTURE_CMD;

  typedef enum GESTURE_STATE 
  {
    GESTURE_STATE_BEGIN   = 1, // starts
    GESTURE_STATE_INERTIA = 2, // events generated by inertia processor
    GESTURE_STATE_END     = 4, // end, last event of the gesture sequence
  } GESTURE_STATE;

  typedef enum GESTURE_TYPE_FLAGS // requested 
  {
    GESTURE_FLAG_ZOOM               = 0x0001,
    GESTURE_FLAG_ROTATE             = 0x0002,
    GESTURE_FLAG_PAN_VERTICAL       = 0x0004,
    GESTURE_FLAG_PAN_HORIZONTAL     = 0x0008,
    GESTURE_FLAG_TAP1               = 0x0010, // press & tap
    GESTURE_FLAG_TAP2               = 0x0020, // two fingers tap

    GESTURE_FLAG_PAN_WITH_GUTTER    = 0x4000, // PAN_VERTICAL and PAN_HORIZONTAL modifiers
    GESTURE_FLAG_PAN_WITH_INERTIA   = 0x8000, //
    GESTURE_FLAGS_ALL               = 0xFFFF, //
  } GESTURE_TYPE_FLAGS;

  typedef struct GESTURE_PARAMS
  {
    UINT      cmd;          // GESTURE_EVENTS
    HELEMENT  target;       // target element
    POINT     pos;          // position of cursor, element relative
    POINT     pos_view;     // position of cursor, view relative
    UINT      flags;        // for GESTURE_REQUEST combination of GESTURE_FLAGs. 
                            // for others it is a combination of GESTURE_STATe's
    UINT      delta_time;   // period of time from previous event.
    SIZE      delta_xy;     // for GESTURE_PAN it is a direction vector 
    double    delta_v;      // for GESTURE_ROTATE - delta angle (radians) 
                            // for GESTURE_ZOOM - zoom value, is less or greater than 1.0    
  } GESTURE_PARAMS;

  typedef enum EXCHANGE_CMD {
    X_DRAG_ENTER = 0,       // drag enters the element
    X_DRAG_LEAVE = 1,       // drag leaves the element  
    X_DRAG = 2,             // drag over the element
    X_DROP = 3,             // data dropped on the element  
    X_PASTE = 4,            // N/A
    X_DRAG_REQUEST = 5,     // N/A
    X_DRAG_CANCEL = 6,      // drag cancelled (e.g. by pressing VK_ESCAPE)
    X_WILL_ACCEPT_DROP = 7, // drop target element shall consume this event in order to receive X_DROP 
  } EXCHANGE_CMD;

  typedef enum DD_MODES {
    DD_MODE_NONE = 0, // DROPEFFECT_NONE	( 0 )
    DD_MODE_COPY = 1, // DROPEFFECT_COPY	( 1 )
    DD_MODE_MOVE = 2, // DROPEFFECT_MOVE	( 2 )
    DD_MODE_COPY_OR_MOVE = 3, // DROPEFFECT_COPY	( 1 ) | DROPEFFECT_MOVE	( 2 )
    DD_MODE_LINK = 4, // DROPEFFECT_LINK	( 4 )
  } DD_MODES;
  
  typedef struct EXCHANGE_PARAMS
  {
    UINT         cmd;          // EXCHANGE_EVENTS
    HELEMENT     target;       // target element
    HELEMENT     source;       // source element (can be null if D&D from external window)
    POINT        pos;          // position of cursor, element relative
    POINT        pos_view;     // position of cursor, view relative
    UINT         mode;         // DD_MODE 
    SCITER_VALUE data;         // packaged drag data
  } EXCHANGE_PARAMS;


  typedef enum DRAW_EVENTS
  {
      DRAW_BACKGROUND = 0,
      DRAW_CONTENT = 1,
      DRAW_FOREGROUND = 2,
      DRAW_OUTLINE = 3,
  } DRAW_EVENTS;

  typedef struct SCITER_GRAPHICS SCITER_GRAPHICS;

  typedef struct DRAW_PARAMS
  {
      UINT             cmd;       // DRAW_EVENTS
      HGFX             gfx;       // hdc to paint on
      RECT             area;      // element area, to get invalid area to paint use GetClipBox,
      UINT             reserved;  // for DRAW_BACKGROUND/DRAW_FOREGROUND - it is a border box
                                  // for DRAW_CONTENT - it is a content box
  } DRAW_PARAMS;

  typedef enum CONTENT_CHANGE_BITS {  // for CONTENT_CHANGED reason
     CONTENT_ADDED = 0x01,
     CONTENT_REMOVED = 0x02,
  } CONTENT_CHANGE_BITS;

  typedef enum BEHAVIOR_EVENTS
  {
      BUTTON_CLICK = 0,              // click on button
      BUTTON_PRESS = 1,              // mouse down or key down in button
      BUTTON_STATE_CHANGED = 2,      // checkbox/radio/slider changed its state/value
      EDIT_VALUE_CHANGING = 3,       // before text change
      EDIT_VALUE_CHANGED = 4,        // after text change
      SELECT_SELECTION_CHANGED = 5,  // selection in <select> changed
      SELECT_VALUE_CHANGED = 6,      // value of <select> changed
      SELECT_STATE_CHANGED = SELECT_VALUE_CHANGED, // OBSOLETE, alias of SELECT_VALUE_CHANGED

      POPUP_REQUEST   = 7,           // request to show popup just received,
                                     //     here DOM of popup element can be modifed.
      POPUP_READY     = 8,           // popup element has been measured and ready to be shown on screen,
                                     //     here you can use functions like ScrollToView.
      POPUP_DISMISSED = 9,           // popup element is closed,
                                     //     here DOM of popup element can be modifed again - e.g. some items can be removed
                                     //     to free memory.

      MENU_ITEM_ACTIVE = 0xA,        // menu item activated by mouse hover or by keyboard,
      MENU_ITEM_CLICK = 0xB,         // menu item click,
                                     //   BEHAVIOR_EVENT_PARAMS structure layout
                                     //   BEHAVIOR_EVENT_PARAMS.cmd - MENU_ITEM_CLICK/MENU_ITEM_ACTIVE
                                     //   BEHAVIOR_EVENT_PARAMS.heTarget - owner(anchor) of the menu
                                     //   BEHAVIOR_EVENT_PARAMS.he - the menu item, presumably <li> element
                                     //   BEHAVIOR_EVENT_PARAMS.reason - BY_MOUSE_CLICK | BY_KEY_CLICK


      CONTEXT_MENU_REQUEST = 0x10,   // "right-click", BEHAVIOR_EVENT_PARAMS::he is current popup menu HELEMENT being processed or NULL.
                                     // application can provide its own HELEMENT here (if it is NULL) or modify current menu element.

      VISIUAL_STATUS_CHANGED = 0x11, // broadcast notification, sent to all elements of some container being shown or hidden
      DISABLED_STATUS_CHANGED = 0x12,// broadcast notification, sent to all elements of some container that got new value of :disabled state

      POPUP_DISMISSING = 0x13,       // popup is about to be closed

      CONTENT_CHANGED = 0x15,        // content has been changed, is posted to the element that gets content changed,  reason is combination of CONTENT_CHANGE_BITS.
                                     // target == NULL means the window got new document and this event is dispatched only to the window.

      CLICK = 0x16,                  // generic click
      CHANGE = 0x17,                 // generic change

      // "grey" event codes  - notfications from behaviors from this SDK
      HYPERLINK_CLICK = 0x80,        // hyperlink click

      ELEMENT_COLLAPSED = 0x90,      // element was collapsed, so far only behavior:tabs is sending these two to the panels
      ELEMENT_EXPANDED = 0x91,       // element was expanded,

      ACTIVATE_CHILD = 0x92,         // activate (select) child,
                                     // used for example by accesskeys behaviors to send activation request, e.g. tab on behavior:tabs.

      UI_STATE_CHANGED = 0x95,       // ui state changed, observers shall update their visual states.
                                     // is sent for example by behavior:richtext when caret position/selection has changed.

      FORM_SUBMIT = 0x96,                   // behavior:form detected submission event. BEHAVIOR_EVENT_PARAMS::data field contains data to be posted.
                                     // BEHAVIOR_EVENT_PARAMS::data is of type T_MAP in this case key/value pairs of data that is about 
                                     // to be submitted. You can modify the data or discard submission by returning true from the handler.
      FORM_RESET  = 0x97,            // behavior:form detected reset event (from button type=reset). BEHAVIOR_EVENT_PARAMS::data field contains data to be reset.
                                     // BEHAVIOR_EVENT_PARAMS::data is of type T_MAP in this case key/value pairs of data that is about 
                                     // to be rest. You can modify the data or discard reset by returning true from the handler.

      DOCUMENT_COMPLETE = 0x98,             // document in behavior:frame or root document is complete.

      HISTORY_PUSH = 0x99,                  // requests to behavior:history (commands)
      HISTORY_DROP = 0x9A,
      HISTORY_PRIOR = 0x9B,
      HISTORY_NEXT = 0x9C,
      HISTORY_STATE_CHANGED = 0x9D,  // behavior:history notification - history stack has changed
      
      CLOSE_POPUP = 0x9E,            // close popup request,
      REQUEST_TOOLTIP = 0x9F,        // request tooltip, evt.source <- is the tooltip element.

      ANIMATION         = 0xA0,      // animation started (reason=1) or ended(reason=0) on the element.

      DOCUMENT_CREATED  = 0xC0,      // document created, script namespace initialized. target -> the document
      DOCUMENT_CLOSE_REQUEST = 0xC1, // document is about to be closed, to cancel closing do: evt.data = sciter::value("cancel");
      DOCUMENT_CLOSE    = 0xC2,      // last notification before document removal from the DOM
      DOCUMENT_READY    = 0xC3,      // document has got DOM structure, styles and behaviors of DOM elements. Script loading run is complete at this moment. 
      DOCUMENT_PARSED   = 0xC4,      // document just finished parsing - has got DOM structure. This event is generated before DOCUMENT_READY

      VIDEO_INITIALIZED = 0xD1,      // <video> "ready" notification   
      VIDEO_STARTED     = 0xD2,      // <video> playback started notification   
      VIDEO_STOPPED     = 0xD3,      // <video> playback stoped/paused notification   
      VIDEO_BIND_RQ     = 0xD4,      // <video> request for frame source binding, 
                                     //   If you want to provide your own video frames source for the given target <video> element do the following:
                                     //   1. Handle and consume this VIDEO_BIND_RQ request 
                                     //   2. You will receive second VIDEO_BIND_RQ request/event for the same <video> element
                                     //      but this time with the 'reason' field set to an instance of sciter::video_destination interface.
                                     //   3. add_ref() it and store it for example in worker thread producing video frames.
                                     //   4. call sciter::video_destination::start_streaming(...) providing needed parameters
                                     //      call sciter::video_destination::render_frame(...) as soon as they are available
                                     //      call sciter::video_destination::stop_streaming() to stop the rendering (a.k.a. end of movie reached)

      PAGINATION_STARTS  = 0xE0,     // behavior:pager starts pagination
      PAGINATION_PAGE    = 0xE1,     // behavior:pager paginated page no, reason -> page no
      PAGINATION_ENDS    = 0xE2,     // behavior:pager end pagination, reason -> total pages

      CUSTOM             = 0xF0,     // event with custom name

      FIRST_APPLICATION_EVENT_CODE = 0x100
      // all custom event codes shall be greater
      // than this number. All codes below this will be used
      // solely by application - Sciter will not intrepret it
      // and will do just dispatching.
      // To send event notifications with  these codes use
      // SciterSend/PostEvent API.

  } BEHAVIOR_EVENTS;

  typedef enum CLICK_REASON
  {
      BY_MOUSE_CLICK,
      BY_KEY_CLICK,
      SYNTHESIZED,       // synthesized, programmatically generated.
      BY_MOUSE_ON_ICON,  
  } CLICK_REASON;

  typedef enum EDIT_CHANGED_REASON
  {
      BY_INS_CHAR,  // single char insertion
      BY_INS_CHARS, // character range insertion, clipboard
      BY_DEL_CHAR,  // single char deletion
      BY_DEL_CHARS, // character range deletion (selection)
      BY_UNDO_REDO, // undo/redo
  } EDIT_CHANGED_REASON;

  typedef struct BEHAVIOR_EVENT_PARAMS
  {
      UINT     cmd;        // BEHAVIOR_EVENTS
      HELEMENT heTarget;   // target element handler, in MENU_ITEM_CLICK this is owner element that caused this menu - e.g. context menu owner
                           // In scripting this field named as Event.owner
      HELEMENT he;         // source element e.g. in SELECTION_CHANGED it is new selected <option>, in MENU_ITEM_CLICK it is menu item (LI) element
      UINT_PTR reason;     // CLICK_REASON or EDIT_CHANGED_REASON - UI action causing change.
                           // In case of custom event notifications this may be any
                           // application specific value.
      SCITER_VALUE data;   // auxiliary data accompanied with the event. E.g. FORM_SUBMIT event is using this field to pass collection of values.

      LPCWSTR  name;       // name of custom event (when cmd == CUSTOM)

  } BEHAVIOR_EVENT_PARAMS;

  typedef struct TIMER_PARAMS
  {
      UINT_PTR timerId;    // timerId that was used to create timer by using SciterSetTimer
  } TIMER_PARAMS;



  // identifiers of methods currently supported by intrinsic behaviors,
  // see function SciterCallBehaviorMethod

  typedef enum BEHAVIOR_METHOD_IDENTIFIERS
  {
    DO_CLICK = 0,
/*  remnants of HTMLayout API, not used 
    GET_TEXT_VALUE = 1,
    SET_TEXT_VALUE,
      // p - TEXT_VALUE_PARAMS

    TEXT_EDIT_GET_SELECTION,
      // p - TEXT_EDIT_SELECTION_PARAMS

    TEXT_EDIT_SET_SELECTION,
      // p - TEXT_EDIT_SELECTION_PARAMS

    // Replace selection content or insert text at current caret position.
    // Replaced text will be selected.
    TEXT_EDIT_REPLACE_SELECTION,
      // p - TEXT_EDIT_REPLACE_SELECTION_PARAMS

    // Set value of type="vscrollbar"/"hscrollbar"
    SCROLL_BAR_GET_VALUE,
    SCROLL_BAR_SET_VALUE,

    TEXT_EDIT_GET_CARET_POSITION, 
    TEXT_EDIT_GET_SELECTION_TEXT, // p - TEXT_SELECTION_PARAMS
    TEXT_EDIT_GET_SELECTION_HTML, // p - TEXT_SELECTION_PARAMS
    TEXT_EDIT_CHAR_POS_AT_XY,     // p - TEXT_EDIT_CHAR_POS_AT_XY_PARAMS
    */

    IS_EMPTY      = 0xFC,       // p - IS_EMPTY_PARAMS // set VALUE_PARAMS::is_empty (false/true) reflects :empty state of the element.
    GET_VALUE     = 0xFD,       // p - VALUE_PARAMS 
    SET_VALUE     = 0xFE,       // p - VALUE_PARAMS  

    FIRST_APPLICATION_METHOD_ID = 0x100
  } BEHAVIOR_METHOD_IDENTIFIERS;

  typedef struct SCRIPTING_METHOD_PARAMS
  {
      LPCSTR              name;   ///< method name
      const SCITER_VALUE* argv;   ///< vector of arguments
      UINT                argc;   ///< argument count
      SCITER_VALUE        result; ///< return value
  } SCRIPTING_METHOD_PARAMS;

  // GET_VALUE/SET_VALUE methods params
  typedef struct VALUE_PARAMS 
  {
    UINT         methodID;
    SCITER_VALUE val;
#ifdef __cplusplus
    VALUE_PARAMS(bool do_set) { methodID = do_set? SET_VALUE : GET_VALUE; }
#endif
  } VALUE_PARAMS;

  // IS_EMPTY method params
  typedef struct IS_EMPTY_PARAMS
  {
    UINT methodID;
    UINT is_empty; // !0 - is empty
#ifdef __cplusplus
    IS_EMPTY_PARAMS():is_empty(0) { methodID = IS_EMPTY; }
#endif
  } IS_EMPTY_PARAMS;

  // see SciterRequestElementData

  typedef struct DATA_ARRIVED_PARAMS
  {
      HELEMENT  initiator;    // element intiator of SciterRequestElementData request,
      LPCBYTE   data;         // data buffer
      UINT      dataSize;     // size of data
      UINT      dataType;     // data type passed "as is" from SciterRequestElementData
      UINT      status;       // status = 0 (dataSize == 0) - unknown error. 
                              // status = 100..505 - http response status, Note: 200 - OK! 
                              // status > 12000 - wininet error code, see ERROR_INTERNET_*** in wininet.h
      LPCWSTR   uri;          // requested url 
  } DATA_ARRIVED_PARAMS;



#pragma pack(pop)

#ifdef __cplusplus

  #pragma warning(disable:4786) //identifier was truncated...
  #pragma warning(disable:4100) //unreferenced formal parameter

  namespace sciter
  {
    struct event_handler_raw // "raw", non-reference counted event handler
    {
      event_handler_raw() {}
      
      virtual void detached  (HELEMENT /*he*/ ) { }
      virtual void attached(HELEMENT /*he*/) { }

      // defines list of event groups this event_handler is subscribed to
      virtual bool subscription( HELEMENT he, UINT& event_groups )
      {
         event_groups = HANDLE_ALL;
         return true;
      }

      // handlers with extended interface
      // by default they are calling old set of handlers (for compatibility with legacy code)

      /*virtual bool handle_mouse  (HELEMENT he, MOUSE_PARAMS& params )         { return false; }
      virtual bool handle_key    (HELEMENT he, KEY_PARAMS& params )           { return false; }
      virtual bool handle_focus  (HELEMENT he, FOCUS_PARAMS& params )         { return false; }
      virtual bool handle_timer  (HELEMENT he )                               { return false; }
      virtual void handle_size   (HELEMENT he )                               { }
      virtual bool handle_draw   (HELEMENT he, DRAW_PARAMS& params )          { return false; }
      virtual bool handle_method_call (HELEMENT he, METHOD_PARAMS& params )   { return false; }
      virtual bool handle_event (HELEMENT he, BEHAVIOR_EVENT_PARAMS& params ) { return false; }
      virtual bool handle_data_arrived (HELEMENT he, DATA_ARRIVED_PARAMS& params ) { return false; }

      virtual bool handle_scripting_call(HELEMENT he, SCRIPTING_METHOD_PARAMS& params )
      {
        return false;
      }*/

      virtual bool handle_mouse  (HELEMENT he, MOUSE_PARAMS& params )
        {
          return on_mouse( he, params.target, params.cmd, params.pos, params.button_state, params.alt_state );
        }
      virtual bool handle_key    (HELEMENT he, KEY_PARAMS& params )
        {
          return on_key( he, params.target, params.cmd, params.key_code, params.alt_state );
        }
      virtual bool handle_focus  (HELEMENT he, FOCUS_PARAMS& params )
        {
          return on_focus( he, params.target, params.cmd );
        }
      virtual bool handle_timer  (HELEMENT he,TIMER_PARAMS& params )
        {
          if(params.timerId)
            return on_timer( he, params.timerId );
          return on_timer( he );
        }

      virtual void handle_size  (HELEMENT he )
        {
          on_size( he );
        }
      virtual bool handle_scroll  (HELEMENT he, SCROLL_PARAMS& params )
        {
          return on_scroll( he, params.target, (SCROLL_EVENTS)params.cmd, params.pos, params.vertical );
        }
      
      virtual bool handle_gesture  (HELEMENT he, GESTURE_PARAMS& params )
        {
          return false;                     
        }

      virtual bool handle_exchange(HELEMENT he, EXCHANGE_PARAMS& params)
        {
          return false;
        }
      
      virtual bool handle_draw   (HELEMENT he, DRAW_PARAMS& params )
        {
          return on_draw(he, params.cmd, params.gfx, params.area );
        }

      virtual bool handle_method_call (HELEMENT he, METHOD_PARAMS& params )
        {
          return on_method_call(he, UINT(params.methodID), &params );
        }

      virtual bool handle_som(HELEMENT he, SOM_PARAMS& params) { return false; }

      // notification events from builtin behaviors - synthesized events: BUTTON_CLICK, VALUE_CHANGED
      // see enum BEHAVIOR_EVENTS
      virtual bool handle_event (HELEMENT he, BEHAVIOR_EVENT_PARAMS& params )
        {
          return on_event(he, params.heTarget, (BEHAVIOR_EVENTS)params.cmd, params.reason );
        }

      // notification event: data requested by SciterRequestElementData just delivered
      virtual bool handle_data_arrived (HELEMENT he, DATA_ARRIVED_PARAMS& params )
        {
          return on_data_arrived(he, params.initiator, params.data, params.dataSize, params.dataType );
        }

      virtual bool handle_scripting_call(HELEMENT he, SCRIPTING_METHOD_PARAMS& params )
        {
          return on_script_call(he, params.name, params.argc, params.argv, params.result);
        }


      //
      // alternative set of event handlers (aka old set).
      //
      virtual bool on_mouse  (HELEMENT he, HELEMENT target, UINT event_type, POINT pt, UINT mouseButtons, UINT keyboardStates ) { return false; }
      virtual bool on_key    (HELEMENT he, HELEMENT target, UINT event_type, UINT code, UINT keyboardStates ) { return false; }
      virtual bool on_focus  (HELEMENT he, HELEMENT target, UINT event_type ) { return false; }
      virtual bool on_timer  (HELEMENT he ) { return false; /*stop this timer*/ }
      virtual bool on_timer  (HELEMENT he, UINT_PTR extTimerId ) { return false; /*stop this timer*/ }
      virtual bool on_draw   (HELEMENT he, UINT draw_type, HGFX hgfx, const RECT& rc ) { return false; /*do default draw*/ }
      virtual void on_size   (HELEMENT he ) { }

      virtual bool on_method_call (HELEMENT he, UINT methodID, METHOD_PARAMS* params ) { return false; /*not handled*/ }

      // calls from CSSS! script and TIScript (if it was not handled by method below). Override this if you want your own methods to the CSSS! namespace.
      // Follwing declaration:
      // #my-active-on {
      //    when-click: r = self.my-method(1,"one");
      // }
      // will end up with on_script_call(he, "my-method" , 2, argv, retval );
      // where argv[0] will be 1 and argv[1] will be "one".
      virtual bool on_script_call(HELEMENT he, LPCSTR name, UINT argc, const SCITER_VALUE* argv, SCITER_VALUE& retval) { return false; }

      // notification events from builtin behaviors - synthesized events: BUTTON_CLICK, VALUE_CHANGED
      // see enum BEHAVIOR_EVENTS
      virtual bool on_event (HELEMENT he, HELEMENT target, BEHAVIOR_EVENTS type, UINT_PTR reason ) { return false; }

      // notification event: data requested by SciterRequestElementData just delivered
      virtual bool on_data_arrived (HELEMENT he, HELEMENT initiator, LPCBYTE data, UINT dataSize, UINT dataType ) { return false; }

      virtual bool on_scroll( HELEMENT he, HELEMENT target, SCROLL_EVENTS cmd, INT pos, SBOOL isVertical ) { return false; }

      // ElementEventProc implementeation
      static SBOOL SC_CALLBACK  element_proc(LPVOID tag, HELEMENT he, UINT evtg, LPVOID prms )
      {
        event_handler_raw* pThis = static_cast<event_handler_raw*>(tag);
        if( pThis ) switch( evtg )
        {
            case SUBSCRIPTIONS_REQUEST:
              {
                UINT *p = (UINT *)prms; 
                return pThis->subscription( he, *p ); 
              }
            case HANDLE_INITIALIZATION:
              {
                INITIALIZATION_PARAMS *p = (INITIALIZATION_PARAMS *)prms;
                if (p->cmd == BEHAVIOR_DETACH) {
                  pThis->detached(he);
                }
                else if (p->cmd == BEHAVIOR_ATTACH) {
                  pThis->attached(he);
                }
                return true;
              }
            case HANDLE_SOM:   {  SOM_PARAMS *p = (SOM_PARAMS *)prms; return pThis->handle_som(he, *p);  }
            case HANDLE_MOUSE: {  MOUSE_PARAMS *p = (MOUSE_PARAMS *)prms; return pThis->handle_mouse( he, *p );  }
            case HANDLE_KEY:   {  KEY_PARAMS *p = (KEY_PARAMS *)prms; return pThis->handle_key( he, *p ); }
            case HANDLE_FOCUS: {  FOCUS_PARAMS *p = (FOCUS_PARAMS *)prms; return pThis->handle_focus( he, *p ); }
            case HANDLE_DRAW:  {  DRAW_PARAMS *p = (DRAW_PARAMS *)prms; return pThis->handle_draw(he, *p ); }
            case HANDLE_TIMER: {  TIMER_PARAMS *p = (TIMER_PARAMS *)prms; return pThis->handle_timer(he, *p); }
            case HANDLE_BEHAVIOR_EVENT:   { BEHAVIOR_EVENT_PARAMS *p = (BEHAVIOR_EVENT_PARAMS *)prms; return pThis->handle_event(he, *p ); }
            case HANDLE_METHOD_CALL:      { METHOD_PARAMS *p = (METHOD_PARAMS *)prms; return pThis->handle_method_call(he, *p ); }
            case HANDLE_DATA_ARRIVED:     { DATA_ARRIVED_PARAMS *p = (DATA_ARRIVED_PARAMS *)prms; return pThis->handle_data_arrived(he, *p ); }
            case HANDLE_SCROLL:     { SCROLL_PARAMS *p = (SCROLL_PARAMS *)prms; return pThis->handle_scroll(he, *p ); }
            case HANDLE_SIZE:  {  pThis->handle_size(he); return false; }
            // call using sciter::value's (from CSSS!)
            case HANDLE_SCRIPTING_METHOD_CALL: { SCRIPTING_METHOD_PARAMS* p = (SCRIPTING_METHOD_PARAMS *)prms; return pThis->handle_scripting_call(he, *p ); }
            //OBSOLETE: case HANDLE_TISCRIPT_METHOD_CALL: { TISCRIPT_METHOD_PARAMS* p = (TISCRIPT_METHOD_PARAMS *)prms; return pThis->handle_scripting_call(he, *p ); }
			      case HANDLE_GESTURE :  { GESTURE_PARAMS *p = (GESTURE_PARAMS *)prms; return pThis->handle_gesture(he, *p ); }
            case HANDLE_EXCHANGE: { EXCHANGE_PARAMS *p = (EXCHANGE_PARAMS *)prms; return pThis->handle_exchange(he, *p); }
			      default:
              assert(false);
        }
        return false;
      }
    };

#ifdef CPP11
    // reference counted event handler
    struct event_handler : public event_handler_raw, public sciter::om::asset<event_handler>
    {
      event_handler() { asset_add_ref(); }
      virtual void detached(HELEMENT /*he*/) override { asset_release(); }

      virtual som_passport_t* asset_get_passport() const override { return nullptr; }

      virtual bool handle_som(HELEMENT he, SOM_PARAMS& params)
      {
        if (params.cmd == SOM_GET_PASSPORT)
          params.data.passport = asset_get_passport();
        else if (params.cmd == SOM_GET_ASSET)
          params.data.asset = static_cast<som_asset_t*>(this); // note: no add_ref
        else
          return false;
        return true;
      }
    };
#else 
    typedef event_handler event_handler_raw;
#endif

    //
    // "behavior" is a named event_handler
    // behaviors organized into one global list to be processed
    // automaticly while handling HLN_ATTACH_BEHAVIOR notification
    //

    struct behavior_factory
    {
      behavior_factory(const char* external_name)
        :next(0),name(external_name)
      {
        // add this implementation to the list (singleton)
        next = root();
        root(this);
      }

      // needs to be overriden
      virtual event_handler* create(HELEMENT he) = 0;

      // behavior list support
      behavior_factory* next;
      const char*       name; // name must be a pointer to a static string

      // returns behavior implementation by name.
      static event_handler* create(const char* name, HELEMENT he)
      {
        for(behavior_factory* t = root(); t; t = t->next)
          if(strcmp(t->name,name)==0)
          {
            return t->create(he);
          }
        return 0; // not found
      }
      // implementation of static list of behaviors
      static behavior_factory* root(behavior_factory* to_set = 0)
      {
        static behavior_factory* _root = 0;
        if(to_set) _root = to_set;
        return _root;
      }

    };

    inline void attach_dom_event_handler(HWINDOW hwnd, event_handler_raw* ph)
    {
      int r = SciterWindowAttachEventHandler( hwnd, &event_handler_raw::element_proc, static_cast<event_handler_raw*>(ph), HANDLE_ALL );
      assert(r == SCDOM_OK); (void)r;
    }
    inline void detach_dom_event_handler(HWINDOW hwnd, event_handler_raw* ph)
    {
      int r = SciterWindowDetachEventHandler( hwnd, &event_handler_raw::element_proc, static_cast<event_handler_raw*>(ph));
      assert(r == SCDOM_OK); (void)r;
    }

// NOTE: no 'override' here as BEGIN/END_FUNCTION_MAP can be declared on classes that do not derive from event_handler,
//       see CHAIN_FUNCTION_MAP 
#define BEGIN_FUNCTION_MAP \
    virtual bool on_script_call(HELEMENT he, LPCSTR name, UINT argc, const sciter::value* argv, sciter::value& retval) \
    { \
      aux::chars _name = aux::chars_of(name);

#define FUNCTION_V(name, method) \
      if( const_chars(name) == _name ) \
        { retval = method(argc,argv); return true; }
#define FUNCTION_0(name, method) \
      if( const_chars(name) == _name && argc == 0) \
        { retval = method(); return true; }
#define FUNCTION_1(name, method) \
      if( const_chars(name) == _name && argc == 1) \
        { retval = method(argv[0]); return true; }
#define FUNCTION_2(name, method) \
      if( const_chars(name) == _name && argc == 2) \
        { retval = method(argv[0],argv[1]); return true; }
#define FUNCTION_3(name, method) \
      if( const_chars(name) == _name && argc == 3) \
        { retval = method(argv[0],argv[1],argv[2]); return true; }
#define FUNCTION_4(name, method) \
      if( const_chars(name) == _name && argc == 4) \
        { retval = method(argv[0],argv[1],argv[2],argv[3]); return true; }
#define FUNCTION_5(name, method) \
      if( const_chars(name) == _name && argc == 5) \
        { retval = method(argv[0],argv[1],argv[2],argv[3],argv[4]); return true; }
#define FUNCTION_6(name, method) \
      if( const_chars(name) == _name && argc == 6) \
        { retval = method(argv[0],argv[1],argv[2],argv[3],argv[4],argv[5]); return true; }
#define CHAIN_FUNCTION_MAP(SUPER_T) \
      if(SUPER_T::on_script_call(he, name, argc, argv, retval)) return true;

#define END_FUNCTION_MAP \
      return false; }


  } //namespace sciter

#endif //__cplusplus




#endif

