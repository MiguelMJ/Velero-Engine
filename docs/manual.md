# Guide

### Table of contents

1. [Render System](#render-system)

## Render System

Lighting layers use two different blend modes: the basic one to draw the fog and another to draw the glow of the lights. This is the reason that locking a light layer wont result in the same drawing.

When layer duplication is implemented, a possible solution would be to duplicate the light layer, set all the glow off in the first one and the set the fog off in the second, lock them and change the render state of the second one for the glow. 