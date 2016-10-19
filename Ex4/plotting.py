import numpy as np
import numpy.random as nr
import matplotlib.pyplot as plt
import math

data = np.genfromtxt("results.txt",delimiter=" ")
data = data[1:]

subdata = data[30:86]
packetsizes = subdata[:,1]
times = subdata[:,2]
throughputs = subdata[:,4]

# # plotting time vs packetsize
# fig1 = plt.figure(1)
# plt.plot(packetsizes, times, 'ro-')
# plt.xscale('log')
# plt.yscale('log')
# plt.axis([5,100000, 0.2, 1500])
# plt.title('Transfer time vs packet size (err_prob=0.5)')
# plt.xlabel('Packet size (byte)')
# plt.ylabel('Transfer time (ms)')
# fig1.savefig('timeVpacketsize.png')

# # plotting throughput vs packetsize
# fig2 = plt.figure(1)
# plt.plot(packetsizes, throughputs, 'ro-')
# plt.xscale('log')
# plt.yscale('log')
# plt.axis([8,100000, 40, 200000])
# plt.title('Throughput vs packet size (err_prob=0.5)')
# plt.xlabel('Packet size (byte)')
# plt.ylabel('Throughput (Kbytes/s)')
# fig2.savefig('throughputVpacketsize.png')

subdata = data[86:]
probabilities = subdata[:,0]
times = subdata[:,2]
throughputs = subdata[:,4]

# plotting time vs error probability
fig3 = plt.figure(1)
plt.plot(probabilities, times, 'ro-')
plt.yscale('log')
plt.axis([-0.05,1, 4, 300])
plt.title('Transfer time vs error probability (packet_size=500bytes)')
plt.xlabel('Error probability')
plt.ylabel('Transfer time (ms)')
fig3.savefig('timeVprobability.png')

# # plotting throughput vs error probability
# fig4 = plt.figure(1)
# plt.plot(probabilities, throughputs, 'ro-')
# plt.yscale('log')
# plt.axis([-0.05,1, 200, 15000])
# plt.title('Throughput vs error probability (packet_size=500bytes)')
# plt.xlabel('Error probability')
# plt.ylabel('Throughput (Kbytes/s)')
# fig4.savefig('throughputVprobability.png')