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
        dataTitles = [header[1], header[2], header[3]]
        
        index = 0
        avg = []
        avg.append([])
        avg.append([])
        avg.append([])
        std = []
        std.append([])
        std.append([])
        std.append([])
        for row in reader:
            if index % 2 == 0: # avg
                appendToData(avg[0], row[1])
                appendToData(avg[1], row[2])
                appendToData(avg[2], row[3])
            else: # stddev
                appendToData(std[0], row[1])
                appendToData(std[1], row[2])
                appendToData(std[2], row[3])
            
            index += 1

        createAndShowPlot(title, dataTitles, avg, std)


def autolabel(ax, rects, xpos='center'):
    ha = {'center': 'center', 'right': 'left', 'left': 'right'}
    offset = {'center': 0, 'right': 1, 'left': -1}

    for rect in rects:
        height = rect.get_height()
        ax.annotate('{}'.format(height),
                    xy=(rect.get_x() + rect.get_width() / 2, height),
                    xytext=(offset[xpos]*3, 3),  # use 3 points offset
                    textcoords="offset points",  # in both directions
                    ha=ha[xpos], va='bottom')


def createAndShowPlot(title, dataTitles, avg, std):

    ind = np.arange(len(avg[0]))  # the x locations for the groups
    width = 0.15  # the width of the bars

    fig, ax = plt.subplots()
    rects1 = ax.bar(ind - width, avg[0], width, yerr=std[0],
                    label=dataTitles[0])
    rects2 = ax.bar(ind , avg[1], width, yerr=std[1],
                    label=dataTitles[1])
    rects3 = ax.bar(ind + width, avg[2], width, yerr=std[2],
                    label=dataTitles[2])

    # Add some text for labels, title and custom x-axis tick labels, etc.
    ax.set_ylabel('Iskorišćenost Procesora [%]')
    ax.set_title(title)
    ax.set_xticks(ind)
    ax.set_xticklabels(('32 inst, 1 trk', '64 inst, 1 trk', '128 inst, 1 trk', '32 inst, 16 trk'))
    ax.legend()

    autolabel(ax, rects1, "left")
    autolabel(ax, rects2, "center")
    autolabel(ax, rects3, "right")

    fig.tight_layout()

    plt.show()


fileName = input('Enter file name:')
title = input('Enter chart title:')
readDataFromCsv(fileName, title)