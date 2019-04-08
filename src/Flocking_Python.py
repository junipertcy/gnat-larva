###########################################################################
# March 2019, Orit Peleg, orit.peleg@colorado.edu
# Code for HW3 CSCI 4314/5314 Dynamic Models in Biology
###########################################################################

import numpy as np
import math
import matplotlib.pyplot as plt

class flock():
    def flocking_python(self):
        N = 400 #No. of Boids
        frames = 100 #No. of frames
        limit = 100 #Axis Limits
        L  = limit*2
        P = 10 #Spread of initial position (gaussian)
        V = 10 #Spread of initial velocity (gaussian)
        delta = 1 #Time Step
        c1 = 0.00001 #Attraction Scaling factor
        c2 = 0.01 #Repulsion scaling factor
        c3 = 1 #Heading scaling factor
        c4 = 0.01 #Randomness scaling factor
        vlimit = 1 #Maximum velocity

        #Initialize
        p = P*np.random.randn(2,N)
        v = V*np.random.randn(2,N)

        #Initializing plot
        plt.ion()
        fig = plt.figure()
        ax = fig.add_subplot(111)


        for i in range(0, frames):
            v1 = np.zeros((2,N))
            v2 = np.zeros((2,N))
            sum_1 = ((np.sum(v[0,:]))/N)*c3
            sum_2 = ((np.sum(v[1,:]))/N)*c3
            v3 = np.array([[sum_1], [sum_2]]) #Calculate Average Velocity

            if (np.linalg.norm(v3) > vlimit): #limit maximum velocity
                v3 = v3*vlimit/np.linalg.norm(v3)

            for n in range(0, N):
                for m in range(0, N):
                    if m!=n:
                        r = p[:,m]-p[:,n] #Vector from one agent to the next

                        if r[0] > L/2:
                            r[0] = r[0]-L
                        elif r[0] < -L/2:
                            r[0] = r[0]+L

                        if r[1] > L/2:
                            r[1] = r[1]-L
                        elif r[1] < -L/2:
                            r[1] = r[1]+L

                        rmag = math.sqrt(r[0]**2+r[1]**2) #Distance between agents
                        v1[:,n] = v1[:,n] + c1*r #Attraction
                        v2[:,n] = v2[:,n] - c2*r /(rmag**2) #Repulsion [non-linear scaling]

                v4 = np.random.randn(2,1)*c4 #Add randomness to motion
                v[:,n] = v1[:,n]+v2[:,n]+v4[:,0]+v3[:,0] #update velocity


            p = p+v*delta #update position

            #Periodic boundary
            tmp_p = p

            tmp_p[0, p[0,:]>L/2] = tmp_p[1,p[0,:]> (L/2)] - L
            tmp_p[1, p[1,:] > L/2] = tmp_p[1, p[1,:] > (L/2)] - L
            tmp_p[0, p[0,:] < -L/2]  = tmp_p[0, p[0,:] < (-L/2)] + L
            tmp_p[1, p[1,:] < -L/2]  = tmp_p[1, p[1,:] < (-L/2)] + L

            p = tmp_p
            line1, = ax.plot(p[0, 0], p[1, 0])

            #update plot
            ax.clear()
            ax.quiver(p[0,:], p[1,:], v[0,:], v[1,:]) # For drawing velocity arrows
            plt.xlim(-limit, limit)
            plt.ylim(-limit, limit)
            line1.set_data(p[0,:], p[1,:])

            fig.canvas.draw()

flock_py = flock()
flock_py.flocking_python()
