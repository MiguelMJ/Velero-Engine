# Guide

### Table of contents

1. [Use of pointers](#use-of-pointers)
2. [Inner communications](#inner-communications)
3. [UML Diagrams](#uml-diagrams)

---

## Use of pointers

1. [Native pointers](#native-pointers)
2. [Smart shared pointers](#smart-shared-pointers-(std::shared_ptr))

The main goal of pointers politics is to prevent manual memory management, given the complexity of the project.  Note that we still need to be cautious: even with smart pointers, it's possible to have garbage uncollected (and unable to be collected) if we play with reflexive or symmetric references. 

### Native pointers

1. Are returned from <u>functions whose purpose is to generate instances</u> of an object that will be immediately put under smart memory management. We don't generate smart pointers directly to avoid the step of copying and destroying it. We want to construct them from a native one.
2. Are used when we need a <u>pointer to an existing object that is not under smart memory management</u>. We don't need to worry about deleting anything, but about null pointers. When possible, a *default instance* should be pointed when creating a new pointer of this kind.

### Smart shared pointers (std::shared_ptr)

1. Are used by <u>objects in charge of creating and destroying other objects</u>. We should not have more pointers to an object than strictly necessary.
2. Are used when we need a <u>pointer to an existing object that is under smart memory management</u>. This way, the smart counter stays coherent.

## Inner Communications

1. [Collisions](#collisions)
2. [Component calls](#component-calls)
3. [Messages](#messages)

### Collisions

They represent "physic" in-game interaction.

#### Collider

For an object to be able to collide, it must instantiate a class extending _Collider_, the class that manages collisions. Whenever a collision occurs, the following information is sent to each collider:

* The identifier of whom they have collided with.
* The movement required for them to solve the collision, supposing the other part wouldn't move.

#### Solid

Solid is an instantiation of _Collider_. Its implementation of the collision logic is made in a way that every collision between two solids its corrected in that iteration. This corrections compares the "priority" of each solid to determine which one to move (for example, the player shouldn't be able to push a column but yes a box). Communication happens only when they touch. The only information that the Solid knows is the identifiers of who touches it.

#### Area of Effect (AoE)

AreaOfEffect is an instantiation of _Collider_, that modifies the collision logic, so communication happens only when a collider enters and exits the area. The only information that the AoE knows is the identifier of who enters and who exits.

#### Perceiver

This is the only class with collision logic that does not instantiate _Collider_. The reason is that a Perceiver purpose is only to get the identifier of who collides with it, without calling the other side of the collision logic. Also, as it is not managed by the collision system, its logic must be activated "from the outside".

#### Advantages

* Fastest way of communication _a priori_.

#### Disadvantages

* Extension of given functionalities must be done either by using other (and slower) methods of communication or by custom code, extending on of these classes.

### Component calls

They are used to communicate components belonging to the same entity. They are handled in a single iteration, like collisions. But, as components are generic, the communication is not done directly (a component cannot access another component). The sender component must give the information to the entity, which forwards it to the corresponding component/s.

#### Advantages

* They only require one iteration.
* The communication is done at a higher level, so it's more flexible.

#### Disadvantages

* Information parsing is required.
* A cyclic communication could block the system.
* There could be no receiver.

### Messages

These are the last and slowest form of communication. There are a generalization of what happens in a component call, but with the instances of the class _Messenger_. As Entity is the main class that extends it, messages are mostly used between them. Some messages can be  directly handled by the entity, but usually it forwards them to its components, resulting in a component call.

Messages are the most flexible and secure way of communication for the same reason that they are so slow: they are not processed in the same moment that they are sent (not like the component calls), so there  is a chance that they are not even processed in the same iteration that they are sent, but in the next one.

#### Scripted messages

Messages can also be sent by scripts. However, we should avoid scripts whose only purpose is to send a messages.

#### Advantages

* High level, easiest way to communicate.
* Cyclic communication is not a problem.

#### Disadvantages

* Information parsing is required.
* There could be no receiver.

## UML Diagrams

1. [Stereotype guide](#stereotype-guide)
2. [Base](#base)
3. [Context](#Structural)

### Stereotype guide

* _<<asset\>>_ Classes managed by the Asset Manager.
* _<<auto\>>_ Static functions that manage all instances of their class.
* _<<core\>>_ Classes with the most inner logic of the game. Their _<<auto\>>_  functions are called in the main loop of the game.
* _<<private\>>_ Private heritage (C++).
* _<<protected\>>_ Protected heritage (C++).
* _<<sfml\>>_ Classes implemented by the third party library sfml.
* _<<singleton\>>_ Classes that must have one and only one instance.

### Base

These classes are the building blocks of the Engine. They implement the core logic and functionalitites. 

#### Classes

* _AIController_
* AreaOfEffect
* _Component_
* _Controller_
* _Collider_
* _Drawable_
* Entity
* _Handler_
* Messenger
* Perceiver
* Prototype
* Scene
* _StateMachine_
* _Timed_
* _UserController_

### Engine Context

These classes handle the execution of the program. They are the central structure of the 