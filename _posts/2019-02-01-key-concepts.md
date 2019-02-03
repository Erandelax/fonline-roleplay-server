---
layout: post
title: "Key concepts"
date: 01-02-2019
categories: Devblog
description: |
  In short about plans, character progression, character effects, weapons, weather, global map and AI.
---
So, let's highlight a few key concepts that I really want to achieve during development.

## Action-RPG controls & combat system

Despite all FOnline builds support turn-based combat, it was rarely used at all RP servers. But the default time based combat system that makes you waste action points for walk speed and seconds of waiting between actions... Meh. I just dont like it.

What I am aimed for is a non-targeting time-based system that would require your time not to wait *between* actions, but to wait for these actions to occur. In simple words, I want abilities & action to consume your time for casting instead of waiting. It will prevent situations like "took up all loot for 1ms and waits for 2s before the next action" and convert 'em into "gathers all loot one by one item during 2s and then performs next action" or even "interupted after 0.9s looting only half of planned objects".

And... Yeah. Abilities. Why to limit yourself with "shot", "burst", "aimed shot"? You must be able to smash enemies with your rifle butt, knock out doors with your feet while aiming pistol, throw grenades with delays, etc, etc, etc. 

And even more, why is all your damage applied to enemies *instantly*? Yay, this game have no projectiles so you can actually miss or run away from enemy rocket/grenade! Bad luck. This should be fixed ASAP, don't you think?..

Combine all of those with the ability to actually run around using your WASD and shoot wherever you point your mouse instead of accurate ol' good RPG-style point-n-clicking and you might get a distant view of what I want to see in this game.

## Character progression

I love large progression trees. I love a large variety of skills and progression options. I love the idea of being able to make your character a skillfull god of all things (taking into account the time spent for that development). I honestly don't like current skill-perk system.

Yet I am not sure what I want to see in this game about character progression. Most likely there will be some Skyrim-like "use it and it will be increased" core approach with a slight time based and limited skill regression and unlockable active and passive ability lines for each developed skill line. Still, characters should progress as-you-play, not as-you-grind.

What I can say for sure, you would not be able to increase your shooting skill by cutting rat skins into ropes and you would not need to punch a Gecko 100 000 times to increase your Melee skill.

## Effects

Buffs and debuffs. If you've ever played other RPGs you might notice those kilometer length rows of applied injuries, blessings, passive effects and other stuff over the buff tray.

Currently FOnline have like 10-15 of such things (bodypart damage, item perks, radiation sickness and addictions).

I want more.

I don't want hunger and thirst as a decaying 0...100% bar, I want occasional moodlet "HUNGRY" with defined effect on your stats per stage. I want things like "ENEGIZED" from Nuka-Cola and "BANDAGED" from bandages. As more as possible. 

Still not sure how to implement all of these corrently tho.

## Weapons

I know how to make it on revision 830, still not sure if it's even possible at 476. But - weapon mods. Yeah, I want weapons to be composed of several parts each with it own stats. Replaceable stats. And as a side effect, same type weapons with different stats depending on their parts. Take your time to find the best one!

Since non-targeting mode is engaged, weapons do not obligatory aim the same hex you are pointing at. Fire ten bullets in a row and you'll cover a wide area in front of you instead of just hammering all of them at one point. New projectiles have no range limit as well (except for ballistic fall), so you can blindfire the opposite corner of the map if necessary.

Not sure yet if I want it, but firing guns might actually drop ammo cases & require you to load clips during breaks.

And yeah. It makes no sense to bring a knife for firefight unless you are really close to the enemy. But - I want to add a full variety of melee fight abilities, including disarming and stunning, so melees would not be so useless after all.

## Damage types, evasion and armoring

For now I'm stuck with slicing, crushing, thermal, mental, radiation and biochemical damage types. Slicing stands for all kinds of knifes, teeth and high penetration ammo. Crushing - blunt melee weapons, low penetration/high caliber rounds. Thermal comes from fire, electricity, laser and plasma damage. Radiation - specific laser/plasma weapons and environment radiation sources. Biochemical - ilnesses, some injuries, poisons and neurotoxins. Mental damage affects only mental health and is recieved every time you or your teammates are hit as well as just comes randomly in time during any fight. 

Each weapon might have more than one damage type as well as each armor might give you protection against more than one of those.

All damage below the damage threshold is applied only to your armor HP (after being divided damage resistance). When your armor item breaks damage threshold drops to zero and you are left just with your natural damage resistances. If incoming damage is higher than armor damage threshold, part of it will be applied to your character health (again, after being divided by your + armor damage resistance).

You can completely block incoming damage from melee attacks with "melee block" ability (given that u manage to activate it in time to interupt the attack). Given enough skills your character might evade melee attacks on itself. 

Ranged attack can not be evaded or blocked that way. With non-targeting combat it is much harder to actually hit someone at range (projectile has to cross you character to be considered a hit). Use cover, shoot around the corners and wear armor.

In general, I want to achieve "rock-paper-scissors" state when melee attackers are most effective against ranged shooters (of course not in clear sight), ranged shooters - agains close range shooters and close range shooters - against melees.

## Carrying objects around

I demand to split "maximum weight limit" into actual hand carrying and whole gear hauling limits. First one affects how much weight you can carry in your hand slots, second - how much things you can carry in your inventory and equipment (armor) slot. Carrying is mostly affected by strength, hauling - endurance. 

## Weather

Global weather. As minimum, the whole map will have 3-4 storms moving around on themselves and dropping rains of varying severity at neighboring locations. 

Dark storm events with increased radioactive background.

Given enough time there might be a possibility to implement season change... But thats too early to discuss for now.

## NPCs

I love NPCs. Still, they are almost useless for any RP server.

What I plan to do is to add a "clone NPC" for every player and somehow link his character to it, so that your character will exist in the game even when u are not in the game. Mainly for running shops and crafting things to sustain the whole crafting process in your absence. Still, not sure how to make it so you'd not get killed too fast.

Also, I really want NPC-driven factions and controllable squads. It's not so much about roleplay, more like the Mount & Blade outpost conquering missions... But there must be things to do when u are alone, right?

All the cities are just too barren and empty on every game session. I can't stand to pretend those two guys at the bonfire are the 200-people population just because it's 2 AM and all players gone sleep.

So, given enough time I plan to try to implement some kind of autonomous interactivity for NPCs (if even not talking, then just wandering around from home to work from time to time). The world must live even if all players are dead.

## Global map

Existing interface should be redone from scratch. Still not sure if I'm going to follow TNF-way of quazi-random locations grid instead of the native Fallout style worldmap... But what I can say: major static cities, wildreness with NPCs wandering between locations, deployable & conquerable outposts, and world-wide events - Yes. Too much to work about to speak about it right now.

## Residential AI

I want your character to have basic AI even while you are ingame. That means, heal when possible, hide when necessary, run away on panic, pick up nearby corpse' ammo and walk around when left alone and much more. Of course there will be an option to turn this stuff off...

And yeah. Ol' good "Overwatch cone" mechanics.

*to be continued*