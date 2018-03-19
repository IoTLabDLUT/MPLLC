# QoE-driven and traffic-flow-density-based link scheduling algorithm in LLC protocol for HetVNETs
## Introduction
In order to satisfy the requirements of the variety of network applications and the convergence between network physical media in heterogeneous network, a new link scheduling algorithm based on priority of data, link status and traffic flow density (PTLCSA) is proposed by improving traditional data-link layer protocol in Vehicular Ad Hoc Network(VANET). At first, based on the four types of vehicle data in vehicle networking standard, data priority is proposed to indicate the sending weight of different data. Meanwhile, delay factor and load factor are proposed to evaluate link status in real time. Then, based on data priority, delay factor, load factor and traffic flow density, data priority matching degree (DPMD) of link is proposed, and according to Pareto optimal, the upper bound of non-WSM maximal distribution frequency about IEEE 802.11p link is presented, which provide theoretical foundation for data frames to select links. And then, based on DPMD and the upper bound of non-WAVE Short Message(non-WSM) maximal distribution frequency, PTLCSA is designed and realized, and the time complexity of PTLCSA is analyzed in this paper. Finally, simulation results show that on the premise that satisfying the Quality of Experience(QoE) of most network applications, PTLCSA can reduce the delay of security applications and improve the utilization ratio of links.
## PTLCSA Algorithm
Combining the above content, this section puts forward the new concept of data priority matching degree of the link(DPMD), and puts forward the link scheduling algorithm PTLCSA based on delay and load of the link as well as packet priority on the premise that the traffic flow density is considered, to realize the heterogeneous network convergence at LLC layer, ensure communication requirements of other types of applications on the premise of meeting QoE of security applications, and improve the utilization ratio of links.<br>

![](https://github.com/IoTLabDLUT/MPLLC/raw/master/image/protocol.png)<br>
    protocol structure of PTLCSA<br>
### Design of PTLCSA Algorithm
In PTLCSA, this paper combines data priority with delay and load of links and traffic flow density of vehicle mobile environment to calculate DPMDs of multiple links in heterogeneous network, to dynamically adjust link load and ensure full utilization of link resources on the premise that transmission requirements of WSM is ensured. The structure chart of the algorithm is shown in as follow, in which obtainment of data type, calculation and comparison of DPMD of each link, and calculation of the maximum distribution frequency of non-WSM are key parts of PTLCSA.<br>
![](https://github.com/IoTLabDLUT/MPLLC/raw/master/image/ptlcsa.png)<br>
* Obtainment of data type. Whenever the vehicle wants to send packets, the packet will be transported downward to LLC layer from upper * layer, and LLC layer will carry out judgment of data type according to the priority of the packet. If the packet is WSM, it will be distributed directly into the IEEE 802.11p link, to ensure the transmission requirement and QoE of security applications.
* Calculation and comparison of DPMD of each link. If packets of traditional TCP/IP is transported to LLC layer, namely, non-WSM, the current status of links should be measured and analyzed to select the most appropriate link for the packet to transmit.<br>
'''cpp
//MPLLC.cpp
double getChannelC(Channel* channel, Packet* packet)
{
	double beta = (double)(packet->getPriority() + 1) / 5;//calculate priority
	double rttEle, loadEle;
	double channelC;
	MyQueue* tempSendQueue = channel->getSendQueue();

	channel->send_mtx.lock();
	channel->rtt_mtx.lock();
	if (channel->getNum() == 1)
	{
		rttEle = channel->getRTT() / RTT_80211P;//calculate rtt factor
		loadEle = (double)tempSendQueue->getNowLoad() / (double)(tempSendQueue->size() - 20);//calculate load factor
	}
	else
	{
		rttEle = channel->getRTT() / RTT_LTE;//calculate rtt factor
		loadEle = (double)tempSendQueue->getNowLoad() / (double)tempSendQueue->size();//calculate load factor
	}
	channelC = beta * rttEle + (1 - beta) * loadEle;
	channel->rtt_mtx.unlock();
	channel->send_mtx.unlock();

	return channelC;
}
'''

* Calculation of the maximum distribution frequency of non-WSM. Calculate the maximum distribution frequency and the instantaneous distribution frequency, adn compare them.<br>

![](https://github.com/IoTLabDLUT/MPLLC/raw/master/image/pseudocode.png)<br>
    Pseudo-code of PTLCSA<br>
## Experimental simulation and performance analysis
### Simulation Environment and Settings
The comparison experiment of the performance carries out data transmission on Node A and Node B. Node A is the sending node and Node B is receiving node. Node A communicates with Node B through two links. Link 1 simulates IEEE 802.11p link, and Link 2 simulates LTE link. IEEE 802.11p link is also used for special transmission of WSM. Under the same network environment, different traffic flow densities are set respectively, and comparison experiment is carried out. When the transmission of all packets is completed, the experiment stops.<br>

![](https://github.com/IoTLabDLUT/MPLLC/raw/master/image/model.png)<br>

![](https://github.com/IoTLabDLUT/MPLLC/raw/master/image/linkparameters.png)<br>
In RPA, Node A will send non-WSM in turn to two links for transmission, and transmission of WSM is carried out through Link 1; while in BFaaS, non-WSM can only be sent through Link 2, and WSM can be only transported through Link 1. 
### Comparison and Analysis of Performances of Each Algorithm
#### Simulation Parameters of Performance Comparison Experiment
Aiming at different traffic environments, this experiment set three different traffic flow densities, corresponding to sparse, medium and dense traffic flow. As mentioned above, vehicle status messages, namely, WSM should be exchanged between vehicles to ensure the vehicle safety. So, under the environment that traffic flow is denser, the frequency of transmission of WSM between vehicles is higher, and the proportion of WSM in total packets will be larger. Thus, this experiment reflects different traffic flow densities mainly through the proportion of WSM in the entire packets sample. <br>

For the size of experiment sample, this experiment sets 5000 packets, in which the proportion of WSM indicates the size of traffic flow density, and there are three different degrees of traffic flow density: sparse, medium and dense, respectively corresponding to the proportion of WSM: 25:1, 10:1 and 5:1. At the same time, in order to compare the effect of each algorithm in extreme case, this experiment also sets the case of extremely dense.<br>

![](https://github.com/IoTLabDLUT/MPLLC/raw/master/image/flowdensity.png)<br>

#### Comparison and Analysis of  Transmission Performance of WSM
The figures indicate the transmission delay of WSM under different traffic flow density environments. It shows that in sparse, medium and dense cases, the transmission delay of WSM of PTLCSA  is smaller than RPA and larger than BFaaS. In addition, when the link has congestion, PTLCSA will dynamically adjust the link load, thus to reduce congestion of the link, ensure the link in a relatively good state, and reduce the transmission delay of WSM. As mentioned above, WSM can only be transported through IEEE 802.11p link. Thus, the figures show that when the traffic flow is denser, the performances and qualities of three algorithms is closer, because the quantity of WSM on IEEE 802.11p link is far more than the quantity of non-WSM. When the traffic flow density reaches the extremely dense, namely,  the proportion of WSM in total packets reaches 80%, and three algorithms have almost same transmission performance of WSM. <br>

![](https://github.com/IoTLabDLUT/MPLLC/raw/master/image/linechart/200WSM.jpg)<br>
    (a)WSM delay of sparse density<br>
![](https://github.com/IoTLabDLUT/MPLLC/raw/master/image/linechart/500WSM.jpg)<br>
    (b) WSM delay of medium density<br>
![](https://github.com/IoTLabDLUT/MPLLC/raw/master/image/linechart/1000WSM.jpg)<br>
    (c) WSM delay of dense density<br>
![](https://github.com/IoTLabDLUT/MPLLC/raw/master/image/linechart/4000WSM.jpg)<br>
    (d) WSM delay of extremely dense density<br>

The above figures show that under non-extreme cases, WSM delay of BFaaS is lower than RPA and PTLCSA. This is because that under the situation that the traffic flow is not dense, the frequency of generating WSM is lower, while IEEE 802.11p link in BFaaS is only responsible for transmission of WSM. Thus, IEEE 802.11p link in BFaaS will not have congestion, and the delay of WSM is low. But the low WSM delay of BFaaS is realized at the expense of massive link resources.
#### Comparison and Analysis of Link Utilization Ratio
The tables show that under general cases, the utilization ratio of RPA is the highest for IEEE 802.11p link, reaching above 90%. However, the above firures show that the use of RPA may cause long-term congestion of IEEE 802.11p link, and increase the delay of WSM. The low delay of IEEE 802.11p link of BFaaS is at the expense of low utilization ratio of IEEE 802.11p link. Under general cases, the utilization ratio of IEEE 802.11p link of BFaaS is below 2%. Under the case of sparse traffic flow, the utilization ratio of BFaaS is even less than 1%. This is no doubt extreme waste for network resources. PTLCSA adjusts load of IEEE 802.11p link dynamically, which not only can ensure the delay of WSM at a low level, but also can fully utilize IEEE 802.11p link, to keep the utilization ratio of IEEE 802.11p link approx. 70%. Under the case of extremely dense traffic flow, three algorithms have almost same of utilization ratio of IEEE 802.11p link, reaching above 93%, to show jointly that the denser traffic flow, the closer performance of three algorithms.<br>

IEEE 802.11p link utilization ratio of each traffic flow density<br>
![](https://github.com/IoTLabDLUT/MPLLC/raw/master/image/utilization.png)<br>
#### Comparison and Analysis of Transmission Performance of Data Messages with Different Priority
In order to reflect the impact of PTLCSA on transmission performance of VANET, this section carries out statistics of average delay of total different types of packets, to compare the differences of three algorithms in holistic transmission performance.<br> 

These figures show that under the cases of sparse, medium and dense traffic flow densities, BFaaS may cause the increase of delay of low priority packets, far more than RPA and PTLCSA, while average delay of low priority packets of PTLCSA is smaller than RPA and BFaaS. Under the case of extremely dense traffic flow, average delay of low priority packets of RPA is smaller than BFaaS and PTLCSA. Because in RPA, low  priority packets use IEEE 802.11p link for transmission frequently, and the bandwidth of IEEE 802.11p link is larger than that of LTE link, so the delay of low priority packets is smaller than LTE link, which yet causes that average delay of WSM (priority 3) of RPA is larger than BFaaS and PTLCSA. Based on the above comparison and analysis, it shows that PTLCSA not only can reduce transmission delay of WSM and improve the utilization ratio of link resources, but also reduce the transmission delay of non-WSM, to meet the QoE of various types of applications, and improve transmission performance of whole VANET.<br>

![](https://github.com/IoTLabDLUT/MPLLC/raw/master/image/histogram/200WSM.jpg)<br>
(a) Average delay of sparse density at each priority<br>
![](https://github.com/IoTLabDLUT/MPLLC/raw/master/image/histogram/500WSM.jpg)<br>
(b) Average delay of medium density at each priority<br>
![](https://github.com/IoTLabDLUT/MPLLC/raw/master/image/histogram/1000WSM.jpg)<br>
(c) Average delay of dense density at each priority<br>
![](https://github.com/IoTLabDLUT/MPLLC/raw/master/image/histogram/4000WSM.jpg)<br>
(d) Average delay of extremely dense density at each priority<br>
## Conclusions
Based on the analysis of researches on heterogeneous network convergence, this paper puts forward a link scheduling algorithm PTLCSA for heterogeneous network based on data priority, link status and traffic flow density aiming at the transmission requirements of security services in VANET. PTLCSA ensures the QoE of non-security services on the premise that transmission requirements of security services are  met at first. Meanwhile, NFV is used to carry out unified management of link resources in VANET, which can improve the utilization ratio of link resources. The simulation result shows that PTLCSA can improve the average utilization ratios of links as well as effectively reduce average transmission delay of WSM in VANET, to utilize link resources fully, improve transmission efficiency of whole network, and  improve the performance of VANET further.
