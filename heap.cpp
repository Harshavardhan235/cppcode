How does std::map differ from std::unordered_map in terms of internal structure and performance?

std::map

Internal Structure: Implemented as a self-balancing binary search tree, typically a Red-Black Tree. This structure maintains its elements in sorted order based on their keys.   

Performance:

Insertion, Deletion, Search: O(logN) on average and in worst-case scenarios, where N is the number of elements.

Memory Usage: Generally higher due to the tree's node structure (pointers, color information).

Iteration: Ordered iteration is efficient and guaranteed to be in ascending key order.   

Use Cases: When sorted data is required, or when logarithmic time complexity for operations is acceptable and guaranteed.

std::unordered_map

Internal Structure: Implemented as a hash table. Elements are not stored in any particular order. It uses a hash function to map keys to buckets.   

Performance:

Insertion, Deletion, Search: O(1) on average. In the worst-case (due to hash collisions), it can degrade to O(N), but good hash functions and proper load factors mitigate this.   

Memory Usage: Generally lower than std::map for the same number of elements, but can increase with a poor hash function or high load factor leading to more buckets.

Iteration: Unordered iteration; the order of elements is not guaranteed and depends on the hash function and internal bucket arrangement.   

Use Cases: When fast average-case lookups are paramount and the order of elements is not important.

Explain Liskov Substitution and Interface Segregation Principles with automotive-based examples.

Liskov Substitution Principle (LSP)

Definition: States that objects of a superclass should be replaceable with objects of its subclasses without breaking the application. In simpler terms, if a program uses a base class, it should be able to use a derived class instead without knowing the difference and without encountering unexpected behavior.

Automotive Example:

Consider a Vehicle base class with a method startEngine(). We might have subclasses like Car, Motorcycle, and ElectricCar.

LSP Compliant: If Car and Motorcycle override startEngine() to perform actions like turning an ignition and ElectricCar overrides it to activate an electric motor, a function that takes a Vehicle object and calls startEngine() should work correctly for all of them. The calling code shouldn't need to know the specific type of Vehicle to start its engine.

LSP Violation: If we introduce a Bicycle subclass of Vehicle and its startEngine() method does nothing or throws an error (because bicycles don't have engines), then Bicycle does not substitute for Vehicle in a meaningful way regarding engine starting. Any code expecting a Vehicle to have a functional startEngine() method would break when given a Bicycle.

Interface Segregation Principle (ISP)

Definition: States that clients should not be forced to depend on interfaces they do not use. Rather than one large, monolithic interface, it is better to have many small, specific interfaces.

Automotive Example:

Consider a VehicleControl interface that has methods for startEngine(), openDoors(), raiseWindows(), activateWipers(), and deployAirbags().

ISP Violation: If we have a Car class that implements VehicleControl, it uses all these methods. However, a Motorcycle class also implementing VehicleControl would be forced to implement openDoors(), raiseWindows(), activateWipers(), and deployAirbags(), even though a motorcycle doesn't have doors, windows, or airbags in the same way a car does. This creates unnecessary dependencies and potentially empty or "do-nothing" implementations.

ISP Compliant: Instead, we could break down VehicleControl into smaller, more specific interfaces:

EngineControllable with startEngine()

DoorControllable with openDoors()

WindowControllable with raiseWindows()

WiperControllable with activateWipers()

SafetySystemControllable with deployAirbags()

Now, Car can implement EngineControllable, DoorControllable, WindowControllable, WiperControllable, and SafetySystemControllable.

Motorcycle can implement only EngineControllable and perhaps other motorcycle-specific interfaces, without being burdened by methods it doesn't need. This keeps interfaces lean and clients only depend on what they actually use.

What is the Observer Design Pattern? How would you use it in an Instrument Cluster system?

Observer Design Pattern:

Definition: A behavioral design pattern that defines a one-to-many dependency between objects so that when one object (the "subject" or "observable") changes state, all its dependents (the "observers") are notified and updated automatically.   

Key Components:

Subject (Observable): The object that holds the state and notifies observers when its state changes. It provides methods to attach, detach, and notify observers.

Observer: The objects that are interested in the state changes of the subject. They provide an update method that the subject calls.

Using it in an Instrument Cluster system:

An instrument cluster (dashboard) is a perfect candidate for the Observer pattern because many different gauges, lights, and displays need to react to changes in vehicle data.

Subject (Observable) Examples:

EngineSpeedSensor (RPM data)

FuelLevelSensor (Fuel tank level)

VehicleSpeedSensor (Speed data)

HeadlightSwitch (Headlight status)

DoorSensor (Door open/closed status)

OilPressureSensor (Engine oil pressure)

Observer Examples:

TachometerDisplay (observes EngineSpeedSensor)

FuelGaugeDisplay (observes FuelLevelSensor)

SpeedometerDisplay (observes VehicleSpeedSensor)

HeadlightIndicatorLight (observes HeadlightSwitch)

DoorAjarWarningLight (observes DoorSensor)

LowOilPressureWarningLight (observes OilPressureSensor)

How it works:

Initialization: Each display, gauge, or warning light (Observer) registers itself with the relevant sensor or control (Subject) it needs to monitor. For example, TachometerDisplay would attach() itself to the EngineSpeedSensor.

State Change: When a sensor's data changes (e.g., the EngineSpeedSensor detects a change in RPM), the sensor object's internal state is updated.

Notification: The sensor (Subject) then calls its notifyObservers() method.

Update: Each registered observer (e.g., TachometerDisplay) receives the notification and its update() method is called, allowing it to retrieve the new data from the subject and update its own display accordingly.

Benefits in this context:

Loose Coupling: The sensors (Subjects) don't need to know about the specific displays (Observers) that are using their data. They just notify registered observers.

Modularity: New displays or warning lights can be added or removed easily without modifying the sensor code.

Maintainability: Changes to how a display works don't affect the sensor logic, and vice-versa.

What exception do you get when trying to start an Activity not declared in the manifest?

When trying to start an Android Activity that has not been declared in the AndroidManifest.xml file, you will typically get a android.content.ActivityNotFoundException.


Sources and related content
