# RT-Stat

A real time statistics (hence the RT-Stat) plugin for the Open Ephys plugin-GUI. The module calculates statistics on the firing rates of sorted neurons. The mean and standard deviation are calculated per each neuron, and a histogram is generated. The module has the option of sending out a TTL event when a neuron hits a user-specified threshold. Details regarading the use and feel of the plugin can be found below.

# Installation

Copy the RT-Stats folder to the plugin folder of Open Ephys. Build all plugins by the instructions on the Open Ephys wiki. 

# Use


![screenshot](https://github.com/Claybarn/RT-Stat/blob/master/Screen%20Shot%202017-02-16%20at%2010.42.20%20PM.png)


1. This button in the top left will move you back a sorted neuron
2. This button in the top right will move you forwards a sorted neuron
3. Displays the currently selected sorted ID, corresponds to the number assigned in the spike sorter module
4. This area displays the current status. In this case, RT-Stats has not started to collect data



![screenshot](https://github.com/Claybarn/RT-Stat/blob/master/Screen%20Shot%202017-02-16%20at%2010.42.31%20PM.png)


1. Displays the user set spike and time requirement until the statistics are calculated and a TTL signal can be broadcasted
2. Indicates current state of RT-Stats, In this case, Rt-Stats is still collecting data
3. This area enables the user to set conditions. The user may set the number of spikes (defaults to 100) and amount of time (defaults to 120 seconds) before the statistics are calcuated, the histogram is generated and the TTL event can be broadcasted. A TTL event can be triggered whenever the firing rate is passed a factor (defaults to 1) of its standard deviation. The direction option determines if a TTL will be broadcasted when the firing rate is a factor of the standard deviation above or below the mean.  
4. The Rt-Stat editor window. The sorted button was implemented to prevent the collection of data during the sorting process, as the box or cluster may pick up unintended timestamps skewing the data. Press the button once all neurons have been sorted. The up and down arrows are short cuts to trigger a TTL event if the firing rate is a factor of the standard deviation above the mean or below the mean. The up arrow is for above and the down arrow is for below. 



![screenshot](https://github.com/Claybarn/RT-Stat/blob/master/Screen%20Shot%202017-02-16%20at%2010.42.44%20PM.png)


1. This area indicates the greatest amount of counts found in any one bin of the histogram to provide reference. 
2. This area displays the statistics calculated upon reaching the user set criteria. The bin size and TTL event count are also presented.
3. Click on the number found in this box to change the number of bins in the histogram. Will calculate the new bin size and display it in box 2.



![screenshot](https://github.com/Claybarn/RT-Stat/blob/master/Screen%20Shot%202017-02-16%20at%2011.20.34%20PM.png)


1. To allow the TTL to interact with other modules (in this case a custom arduino ouput module), ensure the output channel corresponds to the other modules input channel. 
2. In this custom arduino module, this box controls the amount of time in milliseconds a pin (in this case pin 13) will be sent high for.


# To Do
1. Save statistics
2. Plot mean, standard deviation and TTL signal threshold on histogram
3. Option to continuously update the average 
4. Clean up code
5. Clean up user interface




