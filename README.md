# ai_demonstration

Demonstration of steering behavior implementation made with Unreal Engine 5.0.3

# Instructions

- Remember to regenerate visual studio files on clone
- The sliders in the bottom left affects their respective stats which can also be changed by clicking on the number and editing the text (only integers are accepted)
- Left click to move the player pawn which is the target for the following mode : Seek, Flee, Pursuit, Evade, Arrival
- Switch between spawning new waypoints and moving existing ones by clicking on the button "change to xxx mode"
- Left click to move waypoint in the following modes : Circuit, Two Ways and one way
- Left click to spawn waypoint in the following modes : Circuit, Two Ways and one way
- For the following modes : One Way, Two ways and Circuit
  - The waypoint that will be moved is indicated by a green cone
  - The waypoint targetted by the AI is in red
- Each mode has its own waypoints

You can change between basic behaviors and path finding by clicking on the bottom left button named either "Path Finding" or "Steering".

# Path finding Description

Pathing is on the right side of the map and has its own waypoints which can't be moved during gameplay.

- One point: the user chooses a target point in the environment. The character then finds
its way trough the maze in order to reach the target point and then stops.
- Several points: the user chooses several points in the environment. The character then
finds its way to reach the first point, then the second point, and so on, and then stops.
- Circuit: the user chooses several points in the environment. The character then finds its
way trough the maze in order to reach the first point, then the second point, and so on,
and then the character returns to the original point and stops.

Left Click is used to select the waypoints for all the above.

# Behavior descriptions

- Seek: the character moves in order to reach a fixed target.
- Flee: the character moves away from a fixed target.
- Pursuit: the character moves in order to intercept a moving target.
- Evade: the character moves in order to evade a pursuer.
- Arrival: the character moves in order to stop on a fixed target.

- Circuit: the character follows a path continuously. The end of the path rejoins the
beginning, so that the character keeps following the same path indefinitely.
- One way: the character follows a path that ends in a certain point. The character "arrives"
at that point and stops.
- Two ways: the character follows a path that ends in a certain point. The character
"arrives" at that point and then starts to follow the path on the opposite direction. When
the character "arrives" at the starting point, it starts to follow the path on the original
direction again.
