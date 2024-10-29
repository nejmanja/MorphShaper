import matplotlib
import matplotlib.pyplot as plt
import numpy as np

import csv

def appendToData(data, csvInput):
    if csvInput == '#DIV/0!':
        data.append(float('nan'))
    else:
        data.append(float(csvInput))

def readDataFromCsv(fileName, title):
    with open(fileName) as csvFile:
        reader = csv.reader(csvFile, delimiter=',')
        header = next(reader)
        dataTitles = [header[1], header[2], header[3], header[4]]
        
        index = 0
        avg = []
        for i in range(4):
            avg.append([])
        std = []
        for i in range(4):
            std.append([])
        for row in reader:
            if index % 2 == 0: # avg
                for i in range(4):
                    appendToData(avg[i], row[i+1])
            else: # stddev
                for i in range(4):
                    appendToData(std[i], row[i+1])
            
            index += 1

        createAndShowPlot(title, dataTitles, avg, std)


def autolabel(ax, rects, xpos):
    ha = {'center': 'center', 'right': 'left', 'left': 'right'}
    offset = {'center': 0, 'right': 1, 'left': -1}

    for rect in rects:
        height = rect.get_height()
        ax.annotate('{}'.format(height),
                    xy=(rect.get_x() + rect.get_width() / 2, height),
                    xytext=(xpos * 6, 4),  # use 3 points offset
                    textcoords="offset points",  # in both directions
                    ha='center', va='bottom', fontsize=8.5)


def createAndShowPlot(title, dataTitles, avg, std):

    ind = np.arange(len(avg[0]))  # the x locations for the groups
    width = 0.2  # the width of the bars

    fig, ax = plt.subplots()
    rects1 = ax.bar(ind - width * 1.5, avg[0], width, yerr=std[0],
                    label=dataTitles[0])
    rects2 = ax.bar(ind - width * 0.5 , avg[1], width, yerr=std[1],
                    label=dataTitles[1])
    rects3 = ax.bar(ind + width * 0.5, avg[2], width, yerr=std[2],
                    label=dataTitles[2])
    rects4 = ax.bar(ind + width * 1.5, avg[3], width, yerr=std[3],
                    label=dataTitles[3])

    # Add some text for labels, title and custom x-axis tick labels, etc.
    ax.set_ylabel('Iskorišćenost Procesora [%]')
    ax.set_title(title)
    ax.set_xticks(ind)
    ax.set_xticklabels(('32 inst, 1 trk', '64 inst, 1 trk', '128 inst, 1 trk', '32 inst, 16 trk'))
    ax.legend()

    autolabel(ax, rects1, -1.5)
    autolabel(ax, rects2, -1.5)
    autolabel(ax, rects3, 1.7)
    autolabel(ax, rects4, 3)

    fig.tight_layout()

    plt.show()


fileName = input('Enter file name:')
title = input('Enter chart title:')
readDataFromCsv(fileName, title)