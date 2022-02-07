#!/usr/bin/env python
# coding: utf-8

# # Boundary conditions 
# 
# ## Adding colloids and walls to the command-line executable
# 
# A separate header file exists in the main directory of the repository, "addObjectsToOpenQmin.h", which contains some example boundary objects such as spherical colloidal inclusions and planar walls, along with anchoring conditions. Users can un-comment and edit lines as needed, then **recompile** openQmin.cc as described in [Installation](Installation) to incorporate these boundaries automatically into the simulation. Note that recompilation is required with each edit to the .h file. 
# 
# ## Preparing a custom boundary file
# 
# Both the command-line and GUI versions of the executable have the ability to read in a user-prepared
# file that can specify objects composed of arbitrary collections of lattice sites and anchoring conditions
# (rather than just being limited to the predefined walls and colloids described in the addObjectsToOpenQmin.h file).
# 
# The input txt file specifying such objects must be very precisely formatted, and in the /tools/ directory
# we have included a simple utility to assist with this (although any other method can be used to generate
# the desired txt file, e.g. via python).  A few examples of these custom boundary files are in the /assets/
# directory; these were all created using the included mathematica notebook.
# 
# The formatting requirements are the following (copying from
# identical information provided in the comments of src/simulation/multirankSimulationBoundaries.cpp):
# 
# * The first line must be a single integer specifying the number of objects to be read in. This is meant in the colloquial English sense, not in the zero-indexed counting sense. So, if you want your file to specify one object, make sure the first line is the number 1.
# * Each subsequent block must be formatted as follows (with no additional lines between the blocks): 
#     * The first line MUST be formatted as `a b c d` where 
#         * a=0 means oriented anchoring (such as homeotropic or oriented planar), a=1 means degenerate Planar
#         * b is a scalar setting the anchoring strength
#         * c is the preferred value of S0
#         * d is an integer specifying the number of sites, $N_b$.
#     * Subsequently, there MUST be $N_b$ lines of the form `x y z C1 C2 C3 C4 C5`, where 
#         * x, y, and z are the integer lattice sites, 
#         * C1, C2, C3, C4, C5 are real numbers corresponding to the desired anchoring conditions:
#             * For oriented anchoring, C1, C2, C3, C4, C5 correspond directly to the surface-preferred Q-tensor: $$ C1 = Q_{xx};\; C2 = Q_{xy};\; C3=Q_{xz};\; C4 = Q_{yy};\; C5=Q_{yz}$$ where one often will set the Q-tensor by choosing a locally preferred director, $\nu^s$, and setting $Q^B = 3 S_0/2 * (\nu^s \nu^s - \delta_{ab}/3)$.
#             * For degenerate planar anchoring the five constants should be specified as, 
# $$
# C1 = \hat{\nu}_x;\;
# C2 = \hat{\nu}_y;\;
# C3 = \hat{\nu}_z;\;
# C4 = 0.0;\;
# C5 = 0.0,
# $$
# where $\nu^s = \{\cos\phi \sin\theta, \sin\phi \sin\theta, \cos\theta\}$
# is the direction to which the LC prefers to be orthogonal.
# 
