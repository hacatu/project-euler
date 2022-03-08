import matplotlib.pyplot as plt

with open("En.csv", "r") as f:
	n, E = zip(*map(lambda s: s.rstrip().split(","), f.readlines()))
n = [2*int(k) for k in n]
E = [float(e) for e in E]

plt.plot(n, E)
plt.xlabel("Players (2n)")
plt.ylabel("Expected turns (E)")
plt.title("E(n) I guess")
plt.savefig("En.png")
plt.show()
plt.clf()

