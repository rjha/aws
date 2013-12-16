---
title: Arduino Battery Research
layout: default
---



# Arduino Battery Research

## History
- created on 26 Nov. 2013

## Issues

First, there are two kind of issues

- Running arduino full time on batteries
- Using batteries as a backup

There are other variations as well, like running on rechargable batteries (charging via wall power supply) or using a solar panel to recharge the batteries when the sun is high. However, lets first take the simplest case, i.e. we are running on wall power supply and we just want batteries as a backup source.

The MCU considerations are 

- we are running Uno, that draws 34mA in power down mode.
- we are not going to bypass the 7805 linear voltage regulators, so we are assuming that we are going to waste power inside 7805 regulator. 
- The voltage requirements are same as assuming the wall power supply (7V-12v, idea case 9V)

 
so why can't we just power the MCU on a 9V battery and be done with it? why do battery research at all? To understand that, lets write down the parameters that affect battery performance.

## Factors

### Self discharge rate
NiMH batteries can lose 1 percent every day. So if we put these kind of batteries as backup then we are going to run out of batteries at the end of 3 months without even using them. The concern here is frequency of recharge. (NiMH are rechargable though). AA alkaline batteries will have a lower discharge rate,
so they may be more suitable for longer usage.

### Voltage
Not all batteries produce the same voltage. Duracell LF1500 (Li-Iron-sulfide) produces nominal voltage of 1.7V whereas Dc1500 produces 1.2V. If we need Voltage V as output then that means putting N batteries in series to produce Nx Nominal voltage = V.

### Capacity
The duracell DC1500 is rated 2450 mAh. That means ideally it can drive 2450 mA loads for 1 hour. The datasheet for DC1500 shows 1C curve to last 1 HR. However please note that the relationship is not linear and voltage is below 1.2 V at 0.8 hours (80% of discharge). The same battery can work for about 10.2 hours when the load is 0.1C. So how long can a battery drive a load? That is given by the capacity of the battery. A 9V battery may be rated 500mAH and it may run out of charge before the 1.2V DC1500.

### C-rating

Not all batteries can drive all loads. Ideally, the battery should be able to drive any load x for y HR if 
xy= capacity of battery. So the question to ask is, if a 500mAH battery can drive 30A kind of load for 500mAh/ (500mA x 60) = 1h/60 = 1 minute? The answer looks plausible but the C-rating of this battery is 500mA only. so we are asking the battery to drive loads at 60C. Can a battery do that? There are safety concerns here. A battery discharging so fast can burst catch fire and burst into flames. Ideally we should be subjecting the battery to maximum 1C loads only.

### Burst rates
what are the burst rates of the battery? GSM modems can load the battery with upto 2A at a pulse rate of 577 us.

### Internal resistance (impedance?)
todo

### Discharge cycles
How long the battery going to last? Every battery has a finite charge/discharge cycles.

### Cost

Obviously! cost is always a criteria in any design.

### Form considerations
There can be things like most battery packs available for AA batteries only. So we may decide in favor of such batteries because of form factor. size can be another factor. If small size is desired, some batteries may not fit the bill.


## Type of batteries

1. LiPo - Lithium Polymer
2. Li-Ion
3. NiMH - Rechargable
4. Alkaline - un-rechargable



## AWS recommendations 

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

## AWS future considerations

* If we can bypass the linear voltage regulator (or use an LTC3525) and get rocket scream board or 
		other BBB then we can drive the power down mode consumption to micro amperes. Now assuming 5 second 
		of wake up time every minute and 2 bursts of GSM modem lasting 10 seconds at 1A, we should require, 
		5 seconds/minute x 60 x 24 = 2 HR of normal operation + 50mhA ~ 150mAh. With wake up happening 
		every 20 minutes, we can cut that down to 15x24 seconds = 0.1HR of normal opeartion + 50mAh ~ 55mAh/day 
		If GSM bursts last for 2 seconds then, we consume ~ 15mAh and we can run for (2200/15) x 0.8 days 
		~ 4 months.


## GSM modem pulse
todo

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

