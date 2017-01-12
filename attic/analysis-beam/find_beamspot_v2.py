# Finding the center of the beam spot using OpenCV library
#
# Author: Nils.Feege@stonybrook.edu

import cv2
import numpy as np
from matplotlib import pyplot as plt

import argparse

# construct the argument parse and parse the arguments
ap = argparse.ArgumentParser()
ap.add_argument("-i", "--image", help = "path to the image file")
ap.add_argument("-r", "--radius", type = int,
                help = "radius of Gaussian blur; must be odd")
ap.add_argument("-c", "--current", type = float,
                help = "magnet current")
args = vars(ap.parse_args())
 
#img = cv2.imread('IMG_2016_07_18T13_19_04_SN_0205.CR2', 0)
image = cv2.imread(args["image"])
orig = image.copy()
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

# the area of the image with the largest intensity value
(minVal, maxVal, minLoc, maxLoc) = cv2.minMaxLoc(gray)
cv2.circle(image, maxLoc, 100, (255, 0, 0), 2)

plt.imshow(image)
plt.xticks([]), plt.yticks([])  # to hide tick values on X and Y axis
plt.show()

 
# display the results of the naive attempt
#cv2.imshow("Naive", image)

# apply a Gaussian blur to the image then find the brightest
# region
gray = cv2.GaussianBlur(gray, (args["radius"], args["radius"]), 0)
(minVal, maxVal, minLoc, maxLoc) = cv2.minMaxLoc(gray)
image = orig.copy()
cv2.circle(image, maxLoc, args["radius"], (255, 0, 0), 2)
 
## display the results of our newly improved method
#cv2.imshow("Robust", image)
#cv2.waitKey(0)

#print maxLoc args["radius"]

print("%f %i %i" % (args["current"], maxLoc[0], maxLoc[1]))

plt.imshow(image)
plt.xticks([]), plt.yticks([])  # to hide tick values on X and Y axis
plt.show()
