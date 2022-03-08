"""
import sympy
a, b, c, d, e, f = sympy.symbols("a b c d e f")
T = (sympy.sqrt((a + 50)**2 + (a + 25*sympy.sqrt(2))**2)/10 +
     sympy.sqrt((b - a)**2 + (b - a - 10*sympy.sqrt(2))**2)/9 +
     sympy.sqrt((c - b)**2 + (c - b - 10*sympy.sqrt(2))**2)/8 +
     sympy.sqrt((d - c)**2 + (d - c - 10*sympy.sqrt(2))**2)/7 +
     sympy.sqrt((e - d)**2 + (e - d - 10*sympy.sqrt(2))**2)/6 +
     sympy.sqrt((f - e)**2 + (f - e - 10*sympy.sqrt(2))**2)/5 +
     sympy.sqrt((f - 50)**2 + (f - 25*sympy.sqrt(2))**2)/10)
"""

from math import sqrt, hypot

"""
Snell's Law should apply, so we have A at (0, 0), B at 50*sqrt(2)*(1, 1),
and the segments of the swamp between the lines y = 25*sqrt(2) - 25, y = 25*sqrt(2) - 15, ..., y = 20*sqrt(2) + 25.
Call the velocity in each of the 7 regions v_0 = v_6 = 10, v_1 = 9, ..., and call the angle of incidence
th_0, th_1, ..., th_6.
sin(th_{i+1})/sin(th_i) = v_{i+1}/v_i by Snell's law, and we know the bases of the right triangles whose hypoteni
are the path travelled add up to the x displacement, and their heights add up to the y displacement.
The heights can be found by multiplying the tangents by the angles of incidence.
50*sqrt(2) = (25*sqrt(2) - 25)*(tan(th_0) + tan(th_6)) + 10*(tan(th_1) + tan(th_2) + tan(th_3) + tan(th_4) + tan(th_5))
Now we need to rewrite Snell's law in terms of tangents.
sin(th_{i+1}) = v_{i+1}/v_i*sin(th_i)
sin(x) = o/h = o/sqrt(o^2 + a^2) = (o/a)/sqrt((o/a)^2 + 1) = tan(x)/sqrt(tan(x)^2 + 1)
th_{i+1} = arcsin(v_{i+1}/v_i*sin(th_i))
tan(th_{i+1}) = tan(arcsin(...))
tan(arcsin(o/h))=o/a = o/sqrt(h^2 - o^2)
tan(th_{i+1}) = tan(arcsin(v_{i+1}*tan(th_i)/(v_i*sqrt(tan(th_i)^2 + 1))))
tan(th_{i+1}) = v_{i+1}*tan(th_i)/sqrt(v_i^2*(tan(th_i)^2 + 1) - v_{i+1}^2*tan(th_i)^2)

Now we can rewrite tan(th_i) as m_i and take the derivative
dm_{i+1}/dm_i = v_{i+1}v_i^2/((v_i^2 - v_{i+1}^2)m_i^2 + v_i^2)^{3/2}

And now if we define
f(m_0) = (25sqrt(2) - 25)(m_0 + m_6) + 10(m_1 + m_2 + m_3 + m_4 + m_5) - 50sqrt(2)
we can take its derivative and find the zero by Newton's method
df/dm_0 (m) = f'(m) = (25sqrt(2) - 25 + 10m_1'(1 + m_2'(1 + m_3'(1 + m_4'(1 + m_5'(1 + (5sqrt(2) - 5)/2*m_6'))))))
m_{0,j+1} = m_{0,j} - f(m_{0,j})/f'(m_{0,j})

"""

vs = (10, 9, 8, 7, 6, 5, 10)

def y(ms):
	return (25*sqrt(5) - 25)*(ms[0] + ms[6]) + 10*sum(ms[1:-1]) - 50*sqrt(2)

def dmi(ms, i):
	return vs[i]*vs[i-1]**2/((vs[i-1]**2 - vs[i]**2)*ms[i-1]**2 + vs[i-1]**2)**(3/2)

def dy(ms):
	return 25*sqrt(2) - 25 + 10*dmi(ms, 1)*(1 + dmi(ms, 2)*(1 + dmi(ms, 3)*(1 + dmi(ms, 4)*(1 + dmi(ms, 5)*(1 + (5*sqrt(2) - 5)/2*dmi(ms, 6))))))

def time(ms):
	return (25*sqrt(5) - 25)*(hypot(1, ms[0]) + hypot(1, ms[6]))/vs[0] + 10*sum(hypot(1, ms[i])/vs[i] for i in range(1, 6))

def nstep(ms):
	ns = [ms[0] - y(ms)/dy(ms)]
	for i in range(1, 7):
		ns.append(vs[i]*ns[i-1]/sqrt((vs[i-1]**2 - vs[i]**2)*ns[i-1]**2 + vs[i-1]**2))
	return ns

ms = [0, 0, 0, 0, 0, 0, 0]

for _ in range(250):
	ms = nstep(ms)
print(time((1, 1, 1, 1, 1, 1, 1)))
print(time(ms))

