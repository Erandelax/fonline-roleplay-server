---
layout: post
title: "Back to work"
date: 31-01-2019
categories: Devblog
---

After a long break, I'm back to FOnline development! Spent dozen of hours fixing own `input.fos` (keyboard & mouse controls handler script) and `gui_menu.fos` (visual menus).

## Popup menus

Looks relatively cool (at least, comparing to old icon based hold-n-scroll context menu):

![WEBP needed](https://res.cloudinary.com/drpez49fp/image/upload/v1548931025/fonline.popup.2.webp)


```cpp
  #include "gui_menu_h.fos"

  void Init(){
    AddContextMenu(MenuAction("Walk here"), @muWalkHere); // Add single menu item

    AddChosenContextMenu(
      Menu("Self...")                 // Create submenu of items
        .Add("Show inventory",@muInventory)
        .Add("Skills")
        .Add("First aid",@muSkillFirstAid)
        .Add("Surgery",@muSkillSurgery)
        .Add(                     // Create subsubmenu of items
          Menu("Animations...")
            .Add("Fall back",@muAnimFallBack)
            .Add("Fall front",@muAnimFallFront)
        )
    );

    AddNpcContextMenu(MenuAction("Call here",@muNpcCallHere));

    AddItemPidContextMenu(PID_CHAIR, MenuAction("Rotate chair",@muItemRotate));
  }

  void muWalkHere(MenuActionResult@ info) {
      uint16 HexX = info.HexX;
      uint16 HexY = info.HexY;
      // ... do smth
  }

  void muSkillFirstAid(MenuActionResult@ info) {
    CritterCl@ cr = info.Critter;
    // ... make self heal <cr>
  }

  // And somewhere at input.fos
  void mouse_down(...) {
    // ...
    if (DIK_RIGHT_MOUSE_BUTTON == button) ContextMenu(__MouseX,__MouseY);
  }
```

## Shooter mode controls & Non-targeting combat

That's just a progress since October (from the Long Journey project). Turn on subtitles visibility for the first one if u need more details.

<iframe width="100%" height="400px" src="https://www.youtube.com/embed/JyKKjhA_06w" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

<iframe width="100%" height="400px" src="https://www.youtube.com/embed/GZVM4TlxmPI" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

<iframe width="100%" height="400px" src="https://www.youtube.com/embed/7x2PJEH59sM" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>