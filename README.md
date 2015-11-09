# README #

Nip Napped is one of the first full-fledged games that I created, and it is one of the largest projects I’ve developed on in
C++. Heavily guided by a tutorial and a roguelike C++ library, I created a roguelike game about my childhood cat, Lucky
(who is still alive and well and living with me!). In short, the game is about how Lucky’s catnip was stolen from Kitty The
Gray and she has to descend into the dungeon in order to conquer Kitty The Gray and retrieve it. Although the majority
of the game framework is from the tutorial that I followed, I added a few of my own parts to the game which includes:

- Stackable items
- Items have a color associated with them in their inventory
- A table-driven monster spawning system that changes depending on what level type you’re on
- Level types
- A “final” boss, Kitty The Gray (levels go on forever and ever and repeat level types)
- Monsters “level up” and become more difficult every time you defeat Kitty The Gray (This is also somewhat table-driven)
- The menu photo (but the little sprite was found here )
- Wherever possible, I avoided magic numbers. In order to do so, I included a “gaming constants” header/source file (needed a source file since they were externs and not all of them were truly constants)

It uses the library [libtcod](http://roguecentral.org/doryen/data/libtcod/doc/1.5.2/index2.html), which is a great library that helps game programmers write a roguelike game.

## Game Controls ##

- [arrows] to move up, left, right, and down
- [g] to pick up an item
- [i] to access your inventory (to use an item)
- [d] to access your inventory (to drop an item)
- [a-z] to use/drop an item in your inventory
- [esc] to go to the menu

## Some Gameplay... ##

![NipNappedDungeon_Screen-1.png](https://bitbucket.org/repo/beGA4B/images/2733661054-NipNappedDungeon_Screen-1.png)

![NipNappedDungeon_Screen-2.png](https://bitbucket.org/repo/beGA4B/images/135878730-NipNappedDungeon_Screen-2.png)

![NipNappedDungeon_Screen-4.png](https://bitbucket.org/repo/beGA4B/images/4144812964-NipNappedDungeon_Screen-4.png)