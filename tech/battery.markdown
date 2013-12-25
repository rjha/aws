---
title: AWS battery and charger research
layout: default
---

AWS battery and charger requirements

# The problem

The prototype AWS circuit is powered by a 12V DC adapter. Arduino Uno board can take 7V-12V. However the demand of a wall 
power supply can't always be met in the field. For that reason we would like to power AWS from solar panels. Even when wall 
power is available we need a rechargeable battery as a backup.

So we have two requirements

- Run aws on solar power
- Add a rechargeable battery + charger to aws kit on wall power 

# AC + Rechargeable battery

so what is the big deal? Big deal is finding 

- suitable batteries (right ratings)
- A smart battery charger

There are questions like selecting NiMH vs. Li-Ion batteries. Batteries have capacity, peak current ratings etc. that we need 
to look at.  We also want the charger inside our unit, a part of our circuit. We need a battery and charge that can be plugged 
into wall supply and left unattended. Unattended battery charging requires that we must adhere to all precautions.

## Right rechargeable battery

To drive an Arduino Uno, we need 7V-12V input. So our battery should be able to provide minimum 7V for development board.
The peak current requirement is 1A so we need a battery with minimum C-rating around 1500mA. The capacity of battery would
depend on intended usage. Suppose we draw 50mA current. A 1500mAh battery should provide backup for 1500/50 = 30 Hours.

When we switch to a low power board, the voltage requirement would come down.

## Why battery charger matters

When you are charging a battery, the circuit should take care of not over-charging the battery. Similarly the circuit 
should also take care of not over discharging the batteries. There are plenty more considerations for a safe charger

+ Protection from overheating
+ No Overcharging
+ No over discharging
+ Protect from  over load
+ Protect from short circuit etc.
+ Balance charging for a battery pack

Bad things may happen to batteries, like catching fire (No, really, that happens!) Now, this should be a big deal when we have
100 Rs. emergency lights that can be charged from wall power? Also, we have 1100 Rs. mobiles with chargers that look pretty 
decent. They even tell you when the battery is full! 

Mobiles have in-built chargers and mass production game is different.  Here we need to put an AC-DC battery charger circuit 
inside our AWS kit. The battery charger can be a ready made unit or it can be built on top of battery management IC.

## Our requirement

- Rechargeable battery capable of providing 5V Vin and 1A current
- Always on charger + battery (like cordless phone units)  

## Question 

- Are battery chargers in 100 Rs. emergency lights a safety hazard?
- Are NiMH batteries safer than LiPo batteries? 


# Solar power

To drive aws circuit on solar panel, we need 3 things. 

- Solar panels
- Right battery 
- A DC-DC battery charger

## Solar panel

You can't just put any solar panel. Every solar panel has a particular wattage. We need a panel that can charge a 
12V battery (so solar panel voltage rating should be > 12V) and provide a peak current of 1A (so we need more than 
12W). Later on we plan to switch to 6V (when we have low power boards) so we would need > 6W panels.  

## Solar battery charger

Solar panel should driveour circuit(load) as well as charge the battery when light is up. When the panel can't provide 
right voltage, battery should drive the circuit. The battery charger should also take care of issues like short circuit,
over charging, over discharging etc. Ideally we should be able to plug solar panel output into charger unit and draw wires
from charger into SLA. The charger unit then should take care of the rest.

## Solar battery

We plan to use deep cycle (kind of batteries that can be charged and discharged frequently) SLA (sealed lead acid) batteries.
To power Arduino Uno development board we need minimum 7V so we would start with 12V batteries. Later on we plan to switch 
to 6V SLA batteries.


## Questions

- Why don't we use LiPo batteries? what is so great about SLA batteries?


# Battery 101

## Self discharge rate

Batteries leak charge when not in use. They can leak all the charge without ever being used. So the rate at which they leak 
charge without being used is important.  NiMH batteries can lose 1 percent every day. So if we put these kind of batteries as 
backup then we are going to run out of batteries at the end of 3 months without even using them. The concern here is frequency 
of recharge. (NiMH are rechargable though). AA alkaline batteries will have a lower discharge rate, so they may be more suitable 
for standalone usage.

## Voltage
Not all batteries produce the same voltage. Duracell LF1500 (Li-Iron-sulfide) produces nominal voltage of 1.7V whereas 
DC1500 produces 1.2V. If we need Voltage V as output then that means putting N batteries in series to produce 
Nx Nominal voltage = required Vin. The required volatge vs. battery voltage would determine the battery pack size.

## Capacity

Duracell DC1500 is rated 2450 mAh. That means ideally it can drive 2450 mA loads for 1 hour. The datasheet for DC1500 shows 
1C curve to last 1 HR. However please note that the relationship is not linear and voltage is below 1.2 V at 0.8 hours 
(80% of discharge). The same battery can work for about 10.2 hours when the load is 0.1C. So how long can a battery drive a 
load? That is given by the capacity of the battery. A 9V battery may be rated 500mAH and it may run out of charge before the 
1.2V DC1500.

## C-rating

Not all batteries can drive all loads. Ideally, the battery should be able to drive any load x for y HR if 
	xy = capacity of battery. 
So the question to ask is, if a 500mAH battery can drive 30A kind of load for 0.5/30 = 1 minute? The answer may look plausible 
but the C-rating of this battery is 500mA only. so we are asking the battery to drive loads at 60C. Can a battery do that? 
There are safety concerns here. A battery discharging so fast can burst catch fire and burst into flames. Ideally we should be 
subjecting the battery to maximum 1C loads only. C-rating is just the number printed on battery, e.g. for a 1500mAh battery,
C-rating is 1500mA.

## Burst rates

what are the burst rates of the battery? GSM modems can load the battery with upto 2A at a pulse rate of 577 us. You need to 
make sure that your battery can drive this kind of bursts.

## Internal resistance (impedance)
todo

## Discharge cycles

How long the battery going to last? Every battery has a finite charge/discharge cycles.

## Cost

Obviously! cost is always a criteria in any design.

## Form considerations

There can be things like most battery packs available for AA batteries only. So we may decide in favor of such batteries 
because of form factor. size can be another factor. If small size is desired, some batteries may not fit the bill.


## Type of batteries

1. LiPo - Lithium Polymer
2. Li-Ion
3. NiMH - Rechargable
4. Alkaline - un-rechargable


## Safety issues

- Don't overcharge - find the voltage for 100% charging
- Battery charging current should not exceed 1C 
- Battery Balancing would be required for a pack. We can use balancing plugs or chargers. 
- Don't load more than discharge rate of a battery called C-rate. LiPo can burst and catch fire.
- while charging batteries 

	* Batteries should be in a fire proof container
	* Batteries should be in a smart charger
	 
- Li-ion and LiPo batteries can explode
- Never charge batteries in series without a smart charger


# AWS recommendations 

* We need to drive our MCU at 7-12V (we are not planning to bypass the linear voltage regulators, 
		no mucking with board), so see the nominal voltage of the battery and decide the number of batteries 
		required and hence the size of pack.
* We need to drive GSM modem pulses, so we need a battery with C rating equivalent to highest current 
		drawn by GSM modem.
* Get a battery with higher capacity 
* Get a battery with low self discharge rate. We plan to provide batteries as backup so users donot 
		have to take them out regularly. 
* NiMH means user have to charge them every 2 months or so but advantage is they are rechargable.
* Alkaline fits the bill of we do not plan to recharge. 
* Assuming 50mA of normal operation and two 10 second bursts for GSM modems at 1A, we would need, 
		24x50mAh + 2x1000x(1/72)mAh ~ 1250 mAh for 24 hours. So a 2200 mAh battery should be good for 
		about 40 hours.
* You can put the batteries in parallel to increase capacity.

# AWS future considerations

* If we can bypass the linear voltage regulator (or use an LTC3525) and get rocket scream board or 
		other BBB then we can drive the power down mode consumption to micro amperes. Now assuming 5 second 
		of wake up time every minute and 2 bursts of GSM modem lasting 10 seconds at 1A, we should require, 
		5 seconds/minute x 60 x 24 = 2 HR of normal operation + 50mhA ~ 150mAh. With wake up happening 
		every 20 minutes, we can cut that down to 15x24 seconds = 0.1HR of normal opeartion + 50mAh ~ 55mAh/day 
		If GSM bursts last for 2 seconds then, we consume ~ 15mAh and we can run for (2200/15) x 0.8 days 
		~ 4 months.


# GSM modem pulse

Find out the burst rates for SIM900 modems.

