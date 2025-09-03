# Honors project | Find your class!

Find your class is specifically designed to assist new students on their first day at school, functioning much like Google Maps but tailored for navigating within a school environment. It serves as a helpful tool for students seeking directions to their respective rooms, providing an intuitive and user-friendly experience! This application aims to ease the navigation process for students, offering a solution to the common challenge of finding the right rooms and directions on their initial day at school.

## What inspired me?

Find your class started as a joke at first during the first group project in my CSC 211 Honors course: Ideathon. There I learned about the subjects that we could try to innovate on and potentially make ideas that would benefit the community. However, at first it didn't go through as pathfinding and trying to understand more complex techniques was just not sitting right on me. Although this may have been true, I just felt like it was itching me to try at least and make something that I never have done before. It also gave me the perspective of when I was a new student and had no idea where I was going for my starting classes. So then, Find your class was born!

## Project Description 

In the broader context, my project focuses on crafting a simple and intuitive digital floor plan. Leveraging tools such as Qt and C++, it create a digital environment where rooms, elevators, and bathrooms are depicted through clear graphics connected by dynamic lines for a visual understanding of the layout. Beyond visuals, Incorporating features like route finding, spatial relationship checks, and dynamic connections, would make it a handy tool for the average new incoming students for their day-to-day tasks. Powered by coordinates (QPoint), visuals (QLabel), and the Qt framework, the aim is to make navigation effortlessly accessible to all users and or students.

## Algorithm

![Diagram drawio](https://github.com/Chimbay/Honors-Project-Find-your-class/assets/127789157/c23c66bc-016f-4cde-aece-9b03b09003ff)

## Challenges that I faced

- Coordinate Challenges:
  - Handling QPoint coordinates initially posed challenges, particularly with negative values. A thorough review of Qt documentation and debugging sessions clarified the positions of coordinate systems, leading to accurate node positioning.
- Pathfinding Logic:
  - Developing a pathfinding algorithm and adjacency checks proved complex due to "having no idea what I was doing". Through testing, debugging, and researching, the logic was developed better, resulting in accurate adjacency checks and a deeper understanding of pathfinding algorithms.
- Widgets:
  - Figuring out how to use widgets at the beginning of developing the GUI application posed the greatest challenges as, again, "having no idea what I was doing". Through watching videos, researching into the QT documentation, C++ documentation, I figured out the UI wasn't the only customizable part of the widgets.

## Accomplishments

- Developed my first GUI application
- Developed my first pathfinding application
- Learned the QT framework and QT API

## What do I plan for the future?

In the future, I plan on expanding this project to be able to handle multiple floors and have a proper node pathfinding algorithm that could find the nearest path more efficiently, especially when mutliple forms of travel are presented. Later on, I hope that I could expand this idea more somewhere.

## Additional information

Google site of the project: [here](https://sites.google.com/view/whereismyclass/home)
