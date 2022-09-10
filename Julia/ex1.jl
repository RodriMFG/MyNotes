#import Pkg;
#Pkg.add("Plots")
using Plots;
f(x) = sin(x)
g(x) = cos(x)
plot([f,g], 0, 2*pi, title="Sin() and Cos()", xlabel="x", ylabel="y")
savefig("ex1.pdf")