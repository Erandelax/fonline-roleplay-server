---
layout: post
title: "A word about character progression"
date: 01-02-2019 17:00:00
categories: Devblog Concepts
---

## Skills and abilities

![Character screen](https://res.cloudinary.com/drpez49fp/image/upload/v1549032413/Progression.png)

There is a set of basic skill lines (melee/ranged combat, etc). Each skill have has the corresponding *static* and *dynamic* parts. 

Dynamic part of each skill is slowly developed every time you perform an action related to this skill. And - slowly degrades in time on itself. 

Static part does not change on itself and never decreases. The reason you have it is that dynamic part will stop to autodecrease as soon as it becomes equal or less than the static part. Still, that is not its "minimum level" because some debuffs might actually make dynamic part to have less points than the static one contains.

Each skill have several ability rows. Abilities (ex Perks) might alter your character stats (passive abs) or give you some new interaction possibilities (like disarming, stunning or weapon breaking active abilities). Each ability might have several progression levels altering their effects. 

Each ability has active part level requirement you must achieve before you can obtain it. Should be noted, that if you already have that ability and your active skill level drops lower than its requirement *ability effect will be altered*, so that actual passive ability buff might become debuff and active ability use attempt might get you hurt. 

I aim to remove both perk and skill points from the game - both for increasing the passive part level and buying abilities you will spend your character experience points. Each next level of passive part, each next level of the ability and each next ability in the row (by the way, you can buy 'em in random order) costs more than previous one. And yeah, you can raise your passive skill part only in range of the active one (otherwise experience cost of such action will be enormous).

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

### Damage types, evasion

For now I'm stuck with slicing, crushing, thermal, mental, radiation and biochemical damage types. Slicing stands for all kinds of knifes, teeth and high penetration ammo. Crushing - blunt melee weapons, low penetration/high caliber rounds. Thermal comes from fire, electricity, laser and plasma damage. Radiation - specific laser/plasma weapons and environment radiation sources. Biochemical - ilnesses, some injuries, poisons and neurotoxins. Mental damage affects only mental health and is recieved every time you or your teammates are hit as well as just comes randomly in time during any fight. 

Each weapon might have more than one damage type as well as each armor might give you protection against more than one of those.

All damage below the damage threshold is applied only to your armor HP (after being divided damage resistance). When your armor item breaks damage threshold drops to zero and you are left just with your natural damage resistances. If incoming damage is higher than armor damage threshold, part of it will be applied to your character health (again, after being divided by your + armor damage resistance).

You can completely block incoming damage from melee attacks with "melee block" ability (given that u manage to activate it in time to interupt the attack). Given enough skills your character might evade melee attacks on itself. 

Ranged attack can not be evaded or blocked that way. With non-targeting combat it is much harder to actually hit someone at range (projectile has to cross you character to be considered a hit). Use cover, shoot around the corners and wear armor.

In general, I want to achieve "rock-paper-scissors" state when melee attackers are most effective against ranged shooters (of course not in clear sight), ranged shooters - agains close range shooters and close range shooters - against melees.

### Carrying objects around

I demand to split "maximum weight limit" into actual hand carrying and whole gear hauling limits. First one affects how much weight you can carry in your hand slots, second - how much things you can carry in your inventory and equipment (armor) slot. Carrying is mostly affected by strength, hauling - endurance. 