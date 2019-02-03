---
layout: post
title: "A word about character progression"
date: 01-02-2019 17:00:00
categories: Devblog Concepts
description: Character skill window concept, character stats, a word about weapons and damage types.
---

## Skills and abilities

![Character screen](https://res.cloudinary.com/drpez49fp/image/upload/v1549032413/Progression.png)

There is a set of basic skill lines (melee/ranged combat, etc). Each skill have the corresponding *static* (green) and *dynamic* (yellow) parts. 

Dynamic part of each skill is slowly developed every time you perform an action related to this skill. And - slowly degrades in time on itself. 

Static part does not change on itself and never decreases. The reason you have it is that dynamic part will stop to autodecrease as soon as it becomes equal or less than the static part. Still, that is not its "minimum level" because some debuffs might actually make dynamic part to have less points than the static one contains.

Each skill have several ability rows. Abilities (ex perks) might alter your character stats (passive abs) or give you some new interaction possibilities (like disarming, stunning or weapon breaking active abilities). Each ability might have several progression levels altering their effects. 

Each ability has active part level requirement you must achieve before you can obtain it. Should be noted, that if you already have that ability and your active skill level drops lower than its requirement *ability effect will be altered*, so that actual passive ability buff might become debuff and active ability use attempt might get you hurt. 

I aim to remove both perk and skill points from the game - both for increasing the passive part level and buying abilities you will spend your character experience points. Each next level of passive part, each next level of the ability and each next ability in the row (by the way, you can buy 'em in random order) costs more than previous one. And yeah, you can raise your passive skill part only in range of the active one (otherwise experience cost of such action will be enormous).

## Experience

You can get experience points from completing quests and contracts, participating in fights and events, capturing outposts and just as a EXP transfer from another player or moderator.

Experience can be spent to level up skill static parts, buying abilities, placing contracts or can
be transfered to another character as token of your appreciation.

Some abilities (like teaching) require some experience to be spent in order to activate it.

Every character earns certain amount of experience every morning. Game moderators and administrators
can reward your character by increasing that amount of points.

*There wont be such thing as progression levels in a game*.

## Character stats

I am quite confused about the list of character & item stats. Just note that everything will change more than once about them, so don't take anything too serious.

### Attributes
- Strength (body)
- Endurance (body)
- Agility (body)
- Charisma (body + mental)
- Perception (mental)
- Intelligence (mental)
- Willpower (mental)

First trio defines your character physical health, last three - mental health.

When character' physical health reaches zero, it falls into critical state. In this state your health is not recovering on its own anymore, you can't participate in fights and if you don't get evacuated into hospital or treated with stimpacks at place - after a while your character will die. Permanently.

When your character' mental health reaches zero you can't perform crafting or any other mental activities and every second you have a chance to drop into states of rage (randomly attacks anyone around) or panic (can't fight, falls down with the same evasion bonuses as in critical state, but not so deadly). Also, it'll take much more time to perform any action than usual.

All abilities are considered as skills and will have their own ability rows.

### Skills & ability rows

Innate skills:

- Strength 
- Endurance
- Agility 
- Charisma
- Perception
- Intelligence 
- Willpower

Acquired skills:

- Melee combat
  - Unarmed fight
  - Blunt weapons (rifle butt smash included)
  - Piercing weapons
  - Cutting weapons
  - Throwing weapons
- Ranged combat
  - Pistols
  - SMGs
  - Shotguns
  - Rifles
  - Assault rifles
  - Machineguns
  - Firearms
  - Beam weapons
  - Plasma weapons
  - Artillery (mortairs, rocket launchers, target lighting)
- Infiltration 
  - Disguise
  - Lockpick
  - Hacking
  - Pickpocket
  - Traps
- Leadership
  - Commanding (squad management & NPC controls)
  - Persuasion (trading, recruiting bonuses, mental buffs creation)
  - Intimidation (mental damage & debuffs creation)
- Medicine
  - Gutting (corpse resource extraction)
  - First aid (in-combat healing)
  - Treatment (long term illnesses treatment)
  - Surgery (serious wounds treatment, implantation)
- Engineering
  - Dismantling (items & objects resource extraction)
  - Melee weapons
  - Ranged weapons
  - Explosives
  - Armors
  - Biochemicals
  - Utilites (misc items)
  - Structures (objects)
  - Robotics (turrets & robots)
- Science (not sure if I need all of those)
  - Researching (get more info about environment, find out recipes for items)
  - Mechanics (+ next ones - various item crafting and decrafting buffs)
  - Electronics
  - Physics
  - Biology
  - Chemistry
- Survival
  - Exploration (get more info about environment & enemies at global map)
  - Navigation (global map buffs)
  - Cooking
  - Taming
  - Farming (fishing and growing)

## Abilities

I wont create a full list of abilities for each skill until fully created and tested the whole skill-ability system. Tho, there are some of them that I'll try to add one way or another:

- Active unarmed ability to detain and hold critters without ropes or handcuffs.
- Active blunt weapons ability to break down or knock out the opponent's weapon.
- Disguise active ability to make yourself visible as another character for other players.
- Commanding abilities to create new permanent settlements and outposts on the global map.
- Structure engineering ability to chose location map (from a set of prebuilts) for such locations.
