# SLiPaD - Smart Lighting and Parking Detection
### Problem
Nowadays, the energy crisis is a constant theme because of the inflated energy prices. [[1]](#1) Furthermore, huge energy consumption is a burden to the environment, as not all means of energy production are non-polluting. According to ”Our World in Data” [[2]](#2), in 2019, 63,3 % of eletrical energy production comes from fossil fuels. It is known that generally, street lamps are continuously switched on at night, most of the time unnecessarily glowing with its full intensity, in the absence of any activities in the street, leading to a great waste of energy. Furthermore, it is in cities where the consequences of using cars are most noticeable. An example of this is the search for a parking space. According to the RAC Foundation [[3]](#3), in England, an average car is parked 95 % of the time, which explains how hard it can get sometimes when trying to find a parking spot. This struggle leads to an increase in carbon dioxide production as well as fuel and energy consumption.

### Project Goal
This project aims the implementation of applications for a Smart City, regarding Smart Lighting and Smart Parking, in order to decrease the energy consumption in public streets, while improving the lives of citizens around the world. The solution will embrace a centralized system, composed by smart street lights capable of turning on only when they detect movement in the surroundings, at night time, and also, capable of detecting available parking spaces in the street post vicinity.

# System Overview

The network architecture to be developed in this project is presented in the following image:

![alt text](https://github.com/TomasLAbreu/slipad/blob/main/report/images/03system_overview/network_arch.png?raw=true)

This is a star topology, in which the gateway relay messages between each local system (lamppost) and a central network server, the remote system. In order for the local systems to communicate with the gateway, LoRa communication is used. This wireless communication takes advantage of the Long Range characteristics of the LoRa physical layer, allowing a single-hop link between the local system and the gateway. All communication modes are capable of bi-directional communication, and there is support for multicast addressing groups. The gateway is connected to the internet in order to store new information about the network in the remote system.

We can divide the system into three subsystems: the local system, which represents a lamppost, the gateway, a device that links the lampposts network to the remote server, and the remote system, that stores information about the lamppost network and allows interaction with the system users by the use of remote client applications. The system overview diagram is shown next.

![alt text](https://github.com/TomasLAbreu/slipad/blob/main/report/images/03system_overview/system_overview.png?raw=true)

### Workflow
This project follows Waterfall model:

- [x] Requirements
- [x] Analysis
- [ ] Design
- [ ] Implementation
- [ ] Testing
- [ ] Deployment
- [ ] Maintenance

Since this project serves educational purposes, the last two phases of the Waterfall model, *Deployment* and *Maintenance*, won't be approached.



## Done by

- José Tomás Lima de Abreu
- Diogo Miguel Cunha Fernandes

Master in Embedded Systems @ Universidade do Minho, 2021/2022

---

## References

<a id="1">[1]</a>  C. Helman, “Energy crisis 2021: How bad is it, and how long will it last?” Forbes, 2021, https://www.forbes.com/sites/christopherhelman/2021/10/19/energy-crisis-2021-how-bad-is-it-and-how-long-will-it-last/?sh=6a5feff14c63 [Accessed on 28/10/2021].

<a id="2">[2]</a> H. Ritchie and M. Roser, “Energy,” Our World in Data, 2020, https://ourworldindata.org/energy [Accessed on 28/10/2021].

<a id="3">[3]</a>  "Cars parked 23 hours a day,” https://www.racfoundation.org/media-centre/cars-parked-23-hours-a-day [Accessed on 8/11/2021]."
