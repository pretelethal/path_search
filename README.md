# path_search

### High Level Overview

This demo application is responsible for finding a path from point a to point b on a grid. More specifically a ground unit finding its destination while avoiding obstacles.

This is done by reading a riskylab formatted json file or by randomly generating a square battle board. The starting board will be presented to the user via CLI output.

The battle unit and destination location shall be placed on the board via CLI interaction by the user. If desired those positions can be randomly generated as well.

Once the board and pieces are configured the path will be determined. This is done by a backtracking based path search algorithm. This will systematically locate all paths from the unit to the destination but only record the shortest viable path it detected.

This application will then output all the x,y coordinates starting with the battle unit and ending with the destination location if a path has been found (in {x,y}, {a,b}, … format). It is possible the unit and/or destination were placed in locations inaccessible to each other which will result in no list of coordinates.

Sample input and output of the application can be found in 'test_io'. Sample json files can be found in 'map'.