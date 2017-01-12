from sympy.solvers import solve
from sympy import Symbol


x = Symbol('x')
Bin = Symbol('Bin')
a = Symbol('a')
b = Symbol('b')
u = Symbol('u')
Bext = Symbol('Bext')

eqn = solve(Bin - 4*u*b**2/((u+1)**2*b**2 - (u-1)**2*a**2)*Bext,u)
print(eqn)

Bin=13
Bext = 15

a= 44.79/2
b = a + .94
eqn = solve(Bin - 4*u*b**2/((u+1)**2*b**2 - (u-1)**2*a**2)*Bext,u)
print(eqn)


