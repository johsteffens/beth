# Beth Graphical Front End

The Graphical Front End (GFE) is an automatic generated and complete graphic user interface for all public elements in a beth-object. It provides ergonomic display, navigation and editing functionality for all elements. 

BGFE uses GTK3 as GUI back-end. (See also: https://www.gtk.org/ )

## Frame

The frame is a GFE-unit associated to another beth-object. All frames belong to the group `bgfe_frame`.

A frame consists of sub-frames associated to sub-objects. Like leaf-object there exists an associated leaf-frame that cannot further be subdivided. In GUI terminology a leaf frame is typically called *Widget*.

### Leaf Frame

Examples leaf-frames are:

* **Check-Box**: Associated with a boolean
* **Entry** (Line-Editor): Associated with a string
* **Label**: Associaled with a constant string

* **Scale** (or Slider): Associated with a number

In BGFE each leaf object (e.g. `f3_t, s2_t, tp_t, sc_t` ) has one naturally associated leaf frame. Many leaf frames can serve different kinds of leave objects, though.

### Composite Frame

Any non-leaf frame is a composite-frame. A composite frame contains a list of frames (leaf or composite). The composite frame is represented by `bgfe_frame_s`. 

The principal purpose of the GFE is to build from any given object the associated frame with a tree-structure dual to the object's tree structure.

### Frame Tree

The frame tree is dual to the associated object-tree. Unlike other beth-objects, each frame are aware of its ancestor and its client. The immediate ancestor is called parent (-frame). For any active frame the ancestry and clients can be retrieved via features.

### Frame Properties 

Each frame has a set of [properties](../../lib/bgfe/bgfe_frame_features_property.emb.x) governing its appearance and interactive behavior. The purpose of the GFE is to compose all properties for a given object with respect to optimizing the user experience.

***User experience*** is a rather subjective notion. Therefore properties must always be customizable to a developers intention. Hence, a secondary GFE purpose is to fashion such customization is a way that minimizes actual GUI development work.

## Client

In BGFE terminology, a client is normally the object-instance to which a specific frame is associated. We call it the 'immediate client'. In a wider sense a client can also be a different object that has some form control over specific immediate-clients. This, for example, could be an application-object, which has many permanent or temporary internal objects but only some of them are subject to the graphical front end.

## Frame-Client Interaction

User actions (like changing and entry, pressing a button, etc) often needs to trigger a program response beyond simply acknowledging the change of the associated variable. Also during the automated frame construction-phase, the client is given the opportunity to configure the frame's specific behavior and appearance.

For this purpose, the frame calls the client via a [client-feature](../../lib/bgfe/bgfe_client.x), which the client may overload. Normally, any such call is escalated to all the client's ancestors until any of them overloads the feature and tells the frame not to further escalate. 

### Syncing

Frame or Client may each initiate a change independently of each other. Such changes need re-synchronization between frame and client. Synchronizing a client's variables to a frame-change (done by the GUI-user) is called `downsync`. Synchronizing a frame to a change in the client's variables (done by the program) is called `upsync`. In short: `downsync` changes the client, `upsync` changes the display. 

`downsync` and `upsync` are member functions of a frame to be called by the client when needed. The client may opt to let BGFE automate part of it.

Nevertheless: How, when and in which thread these syncs appear is fully controlled by the client. A frame will never change the client's state unless the client explicitly allows, handles or initiates it.

A sync automatically propagates to all children of the frame-tree. A clients need not call sync if an ancestor takes care of it. Syncing can also be automated during [Cycling](#cycling).

### Cycling

Since the client controls all interactions between frame and client, the client must tell the frame when to potentially interact. This happens by calling the frame-member function `cycle` periodically. Conceptually, it it is similar to the *event-loop* as typically used in other GUI-like designs.

Just like [synching](#synching), cycling propagates to all ancestors. Hence, only a controlling root object (e.g. on application level) actually calls the cycle function. Depending on the client's needs or preferences, the frame can take care of part or all of the required syncing while `cycle` is being executed. This can be controlled via argument [`action_type`](#action-type).

There is no strict time frame in which to call `cycle`: Infrequent calls can make the application feel unresponsive; frequent calls may waste CPU time. As a rule of thumb:  `5ms`  ... `50ms` between calls is generally a good choice. Sporadic longer delays can be OK. Make sure to suspend the CPU while the client has nothing to do (e.g. via `x_threads_sleep_ms`).

### GUI-Thread

The GUI actually runs in its own dedicated thread to allow seamless GUI-user-actions even while the client is busy. This thread has also its own event-loop, which is different to and fully detached from [Cycling](#cycling).

The GUI-thread is shielded from the client. The client cannot access it and it cannot directly influence the client. Any syncing happens only in the client's thread. Therefore the client need not protect its data from the GUI thread. Any cross-talk, data protection and synchronization between the client and the GUI-thread is handled inside BGFE. A developer need not worry about or even be aware of the GUI-thread.

BGFE maintains two event loops. One is controlled by BGFE and runs in the client's thread the other runs in the GUI's dedicated thread.

### Action Type

The `tp_t` action type is a function-argument used in [`cycle`](#cycling) and [client-features](../../lib/bgfe/bgfe_client.x) to determine how frame-client interaction is to be executed.

The action type can have one of following values: `approve`, `reject`, `acknowledge`, `escalate`, `escapprove`. 

In client features the action type holds a default value (often `escapprove`) but the client can change it to a desired one.

The `cycle` function accepts it as argument to determine what should be the default behavior (e.g. for client features).

## Client Event 

A client event is a client-callback via  [client-features](../../lib/bgfe/bgfe_client.x) . The frame issues the event:

* During construction to give the client a chance for frame configuration.
* In response to a user action.
* In response to a client modification.

 The client may take any action during the callback and it can instruct the frame to perform further action via parameter `action-type`.

### Change-request

Instructs the client about a requested change of a variable due to user action.

**Action-Type**

* `approve`: Approves the request and instructs the frame to execute the change;
* `acknowledge`: Acknowledges the request but the client does the required `downsync` itself.
* `reject`: The client shall not be changed. The frame will undo the editing which triggered the request.
* `escalate`: Like `acknowledge` but the client asks the frame to pass the same request up to its parent. If no ancestor implements the feature, it is generally treated as `reject`.
* `escapprove`: Like `escalate` but when no ancestor implements the feature, it is treated as `approve`.

### Change-confirm

A client-event instructing the client that a change has just been made. The event follows an approve `change-request` or  `downsync`.

**Action-Type**

* `approve`, `acknowledge` : Acknowledges the confirmation.
* `escalate`, `escapprove`: same as in [change-request](#change-request)

* `reject`: Not allowed in this event.



-----------------------------------

<sub>&copy; 2025 Johannes B. Steffens</sub>
