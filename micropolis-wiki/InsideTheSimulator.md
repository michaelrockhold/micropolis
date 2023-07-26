# Inside The Simulator #

## How the Simulator Works and Strategies for Using It ##

Many factors influence the chance of your city's prospering or
floundering: both internal factors (the structure and efficiency of
your city) and external factors (the regional economy, disasters,
etc.).

### Zones ###

Your city is divided up into three primary zones: residential,
commercial and industrial. These zones symbolize the three basic
pillars upon which a city is based: population, industry, and
commerce. All three are necessary for your city to grow and thrive.

  * Residential Zones are where the Sims live. Here they build houses, apartments and community facilities such as churches and schools. Sims are the work force for your city's commercial and industrial zones.

  * Industrial Zones are used to site warehouses, factories, and other unsightly and polluting structures which have a negative impact on surrounding zones. One of the major goals of planning is to separate these "nuisances" from the areas where people live. In this simulation, industrial zones represent the "basic" production of your city. Things produced here are sold outside the city to an "external market," bringing money into the city for future growth.

  * Commercial Zones represent the retail stores and services in your city, including gas stations, grocery stores, banks, and offices. Commercial areas are mainly dedicated to producing goods and services needed within your city. This is called "non-basic" production or production for the "internal market."

### Population -- Residential ###

The major factors controlling residential population are birthrate,
availability of jobs and housing, unemployment, and quality of life
within the city.

Birthrate as used here, is actually a combination of the birthrate
(positive) and the deathrate (negative). Within Micropolis there is
always a positive birthrate.

Availability of jobs (the employment rate) is a ratio of the current
commercial and industrial populations to the total residential
population. As a rule of thumb, the number of commercial and
industrial zones together should roughly equal the number of
residential zones.

If there are more jobs in your city than residents, new settlers will
be attracted. If the job market declines during a local recession,
your people will migrate away in search of jobs.

Housing for your residents is built in the residential zones. These
zones must be powered and connected to the places of employment by
roads. The structures built in residential zones are influenced by
land value and population density.

Quality of life is a measure of relative "attractiveness" assigned to
different zone locations. It is affected by negative factors such as
pollution and crime, and positive factors such as parks and
accessibility.

### External Market -- Industrial ###

There are thousands of variables that influence your city. All these
variables can be influenced by your actions with the exception of one.

The external market (the economic conditions that exist outside of
your city) is controlled by the simulation -- there is nothing you can
do to change it. In many ways, this external market is the original
source of all city growth. Towns frequently begin as production
centers (steel towns, refineries, etc.) that service a demand in the
surrounding region. As time passes, the external market grows to
reflect the regional growth going on around your city.

The industry in your city will attempt to grow as the external market
grows. For this to happen there must be room for expansion (more
industrial zones) and an adequate labor supply (more residential
zones).

### Internal Market -- Commercial ###

The internal market is completely influenced by the conditions within
your city. Internal production, created in the commercial zones,
represents all the things which are purchased and consumed within the
city. Food stores, gas stations, retail stores, financial services,
medical care, etc. -- all depend on a nearby population to service.
Within Micropolis, the size of the internal market determines the rate at
which commercial zones will prosper. Commercial zones need enough
zoned land to build on and an existent, sufficient work force to
employ. The structures built in commercial zones are mainly influenced
by land value and population density.

Commercial zones grow and develop to serve the expanding internal
market. Commercial growth will usually be slow at first, when the
population is small and needs very little. As your city grows,
commercial growth will accelerate and the internal market will become
a much larger consumer of your total city production. This
accelerating effect, when the external/industrial production is
overtaken by the accelerating internal/commercial sector, can turn a
sleepy little town of 50,000 into a thriving capital of 200,000 in a
few short years.

### Tax Rate ###

The tax rate you set controls the amount of income generated by your
city. As taxes are collected each year (simulation time), the Budget
Window will appear, giving you the fiscal details of your city and a
chance to adjust rates. The simulation determines the amount of
revenue collected by assessing each zone an amount based on its land
value, current level of development and the current tax rate.

The tax rate has a global affect on your city's growth rate. If you
set it low (0%-4%), growth will be brisk but the city income will be
low. If you set it high (10%-20%), you will collect a lot in the short
run but in the long run tax income will decrease along with the
population. You must keep tax income high enough to invest in new
development, but low enough not to scare off residents and businesses.
A high tax rate is one way to control city growth, should you want to
experiment with "growth control measures."

### Budgeting ###

City budgeting affects the way your city grows. City infrastructure
cost is represented by three departments: police, fire, and
transportation. You may set the funding levels separately for each.
All three departments will request a certain level of funding each
year. You may supply all or part of the requested funds, in the
attempt to balance safety needs and budgetary concerns.

  * Police Department: Police stations lower the crime rate around a territory. The effective radius of your police station is related to the amount of funding allocated to the police department. Police stations cost $100 per year to fund.

  * Fire Department: Fire stations prevent and extinguish fires. The level of funding determines the effective radius of a fire station. Fire stations put out fires within this radius much sooner than outside it, and decrease the chance that they will start in the first place. Fire stations cost $100 per year to fund.

  * Transportation Department:When you build roads and rail systems you are charged for construction and yearly maintenance. The larger your transportation network, the more it will cost for upkeep. If you decide not to or are unable to pay this maintenance cost, roads will slowly deteriorate and become unusable. The maintenance cost for each piece is: Road - $1, Bridge - $4, Rail - $4, Rail tunnel - $10.

### Power ###

Electrical power makes modern cities possible. Efficient and reliable
power transmission to all zones is the goal of good "power
management."

The entire power grid of your city is periodically checked in the
simulation for links to power. If a zone is connected (by other zones
or power lines) to a power plant, the zone is considered powered.

Zones must be powered for development to occur. Many things (such as
fires, tornadoes, earthquakes and bulldozers) can knock down power
lines and cause blackouts in parts of your city. Development will stop
in unpowered zones, and if power is not quickly restored, the zone
will decline back to its original state of emptiness.

Redundant power plants and power connections can make your power grid
more reliable, but running more line adds construction costs.

### Transportation -- Traffic ###

One of the most important elements of city structure is the
transportation network. It moves Sims and good throughout your city.
Roads typically occupy as much as 25%-40% of the land in urban areas.
Traffic along these roads indicates which sections of your road system
are used the most.

Traffic levels are simulated by a process known as "Trip Generation."
Over time, each populated zone in the city will generate a number of
trips, depending on the population. Each generated trip starts at the
origin zone, travels down the road, and if a "proper destination" is
reached, ends at the destination zone -- otherwise, the trip fails.
Trip failure indicates inaccessibility of a zone and limits its
growth.

The majority of generated trips represent people commuting to and from
work. Additional traffic is generated by residents traveling to
shopping, recreation, etc. When analyzing traffic, the simulator tests
the following traffic routes:

_Traffic Patterns_

  * From Residential
    * To Commercial
    * To Industrial

  * From Commercial
    * To Residential
    * To Industrial

  * From Industrial
    * To Residential

When Sims drive away from an origin zone, they have a limited "trip
range" in which to find a destination zone. Heavy traffic decreases
the trip range. If the destination zone is too far away, the trip is
unsuccessful. Repeated unsuccessful trips will cause the Sims to move
out of the origin zone.

Each road has a limited capacity for traffic. When this capacity is
exceeded traffic jams will form. Traffic jams drastically lower the
capacity of a road, compounding the problem and frustrating drivers.

Traffic conditions fluctuate quickly. Avoid traffic problems by
providing several routes for the traffic to take.

A road must be adjacent to a zone for the zone to be connected to the
traffic pattern. Zones do not conduct traffic the way they conduct
power.

### Pollution ###

Pollution levels are tracked in all areas of your city. This is a
general "nuisance level" that includes air and water pollution, noise
pollution, toxic wastes, etc. Pollution has a negative impact on the
growth of residential areas.

The primary cause of pollution is industrialized zones. The level of
pollution created by an industrial zone increases with its level of
growth.

Traffic is another cause of pollution. There are limited means of
combating the pollution level. Lowering traffic density, limiting
industrial development, and separating the pollution from the
residential areas will help.

### Crime ###

Crime rates are influenced by population density, local law
enforcement, and land values. As population density increases in an
area, the number of crimes committed increases. Crime will also
increase in areas of low land value.

The most effective way to deal with high crime rates is to introduce a
police station into the area. Based on its level of funding, the
police station will reduce the rate of crime in its sphere of
influence. A long-term approach to lowering crime is to raise the land
value of the area. One way to do this is to demolish and rezone (urban
renewal).

### Land Value ###

Land value is one of the most fundamental aspects of urban structure.
The land value of an area affects how that area is used. In this
simulation the land value of an area is based on terrain,
accessibility, pollution, and distance to downtown.

The farther the residents have to go to work, the lower the land value
where they live, due in part to transportation costs. The value of
commercial zones depends greatly on accessibility by the populace.

Land value is also affected by surrounding terrain. If land is closer
to water, trees, agricultural areas, or parks, its value will rise.
Creative placement of zones within the terrain, with little
bulldozing, can make good use of this natural advantage.

Land value and crime rate have a feedback effect on each other. Lower
land values cause crime rates to rise. Higher crime rates cause land
values to drop, and can cause "transition areas" near your central
city to rapidly decline in value.