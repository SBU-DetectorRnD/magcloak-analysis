import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import curve_fit

def expfunc(x, a, b, c):
    return a * (1 - np.exp(-b * x))

x = np.linspace(0,4,50)
y = expfunc(x, 2.5, 1.3, 0.5)
yn = y + 0.2*np.random.normal(size=len(x))

popt, pcov = curve_fit(expfunc, x, yn)

print popt[0]

plt.figure()
plt.plot(x, yn, 'ko', label="Original Noised Data")
plt.plot(x, expfunc(x, *popt), 'r-', label="Fitted Curve")
plt.legend()
plt.show()
