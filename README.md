# ai_demonstration

Demonstration of steering behavior implementation done on Unreal Engine 5.0.3 

# Instructions

- Remember to regenerate visual studio files on clone
- The sliders in the bottom left affects their respective stats which can also be changed by clicking on the number and editing the text (only integers are accepted)
- Left click to move the player pawn which is the target for the following mode : Seek, Flee, Pursuit, Evade, Arrival
- Left click to move waypoint in the following modes : Circuit, Two Ways and one way
- Add more waypoint to Circuit, One Way or Two Ways by clicking on +1 while in corresponding mode, the new waypoint will spawn under the AI
- For the following modes : One Way, Two ways and Circuit
  - The waypoint that will be moved is indicated by a green cone
  - The waypoint targetted by the AI is in red

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
