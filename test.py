import matplotlib.pyplot as plt
from math import log, exp

nmap = {0: ' insertion_sort', 1: ' selection_sort', 2: ' quick_sort',
        3: '  shell_sort', 4: ' merge_sort', 5: ' intro_sort', 6: ' heap_sort'}

cmap = {0: 'r', 1: 'b', 2: 'g', 3: 'c', 4: 'm', 5: 'y', 6: 'k'}

plt.grid(True)
plt.figure(figsize=(40, 20))
plt.locator_params('x', nbins=20)
plt.locator_params('y', nbins=5)
plt.xlabel("X = N")  # 横轴的标签F
plt.ylabel("Y = (ms)^-1")  # 纵轴的标签
plt.title("C++ sort")  # 图像标头


f = open("log", "r")
all = []
data = f.read().split()

for elem in data:
    try:
        all.append(float(elem))
    except ValueError:
        pass

pp = open("test_data", "r").read().split()
N = int(pp[-1])
step = 2 ^ (int(N/10))

# stx = []
# sty = []
# for k in range(int(pp[0]), int(pp[0])+N):
#     stx.append(k)
#     sty.append(0.0001*k*log(2, k))

# plt.scatter(stx, sty)
# plt.plot(stx, sty, 'b')


for i in range(7):

    mm = all[N*2*i: N*2*(i+1)]
    mx = mm[::step]
    my = mm[1::step]
    plt.text(mx[-1], my[-1], nmap[i])
    plt.text(mx[0], my[0], nmap[i])

    plt.scatter(mx, my)  # 画点，将列表中的值分别画在plot中
    plt.plot(mx, my, color=cmap[i])  # 连线，按顺序将

plt.savefig('result.png')

all.clear()
f.close()

# plt.savefig('shit.png')
# f.close()
