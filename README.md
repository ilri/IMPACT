# IMPACT
A number of studies have attempted to understand the impact of cropping and livestock activities on the livelihoods of smallholder farmers. In particular, research has been focused on how farmers' choices are shaped and constrained by the socio-economic, environmental, and socio-political characteristics of the ecoregion they live in. Although standard datasets have been established for components of systems (crops, livestock, or soil), little effort has been made to produce standard system-level datasets. In light of this, Impact was created to provide a unifying framework for collecting system-level information in a standard format for any tropical farming system. In addition, Impact was developed to provide standard input files and data exchange protocols to run and link models for assessing the impacts of alternative management or policy interventions on tropical smallerholder farming systems.

Impact can be used by anyone who wishes to characterise a tropical farming system in such a way that they can then better and more clearly understand how the system works. By characterising a farming system, a user will be able to test within Impact the effects of different management scenarios on the system and to understand how they impact on a number of aspects of the farm, e.g., food security, income. At a simpler level, Impact also enables a user to see clearly the effects of an existing management strategy on a farming system.

IMPACT is cross platform. It can be build on Windows, Linux and Mac.

![Image](/screenshot.jpg?raw=true "Screen-shot")

## Technology
IMPACT was built using:

- [C++](https://isocpp.org/), a general-purpose programming language.
- [Qt](https://www.qt.io/), a cross-platform application framework.
- [SQLite](https://www.sqlite.org/), a relational database management system contained in a C programming library.
- [HighCharts](http://www.highcharts.com/), a charting library written in pure JavaScript. *(Included in source code)*


## Building and testing
To build IMPACT on Linux do:

    $ git clone https://github.com/ilri/IMPACT.git
    $ cd IMPACT

Build the IMPACT widgets

    $ cd plugins/designer/impwidgets
    $ qmake
    $ make

Build the System Characterisation plugin

    $ cd ../../syschar/impmainmodules/
    $ qmake
    $ make    

Build IMPACT

    $ cd cd ../../..
    $ qmake
    $ make    

## Author
Carlos Quiros (cquiros_at_qlands.com / c.f.quiros_at_cgiar.org)


## License
This repository contains the code of:

- [HighCharts](http://www.highcharts.com/) which is licensed under the [Creative Commons Attribution Non-Comercial](http://creativecommons.org/licenses/by-nc/3.0/).


Otherwise, IMPACT is licensed under [GPL V3](http://www.gnu.org/licenses/gpl-3.0.html).
