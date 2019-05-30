'''

Code adapted and changed to this application from: 

Kalman Filters and Random Signals in Python by Roger Labbe is licensed under a
Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
based on work at:
https://github.com/rlabbe/Kalman-Filters-and-Random-Signals-in-Python

'''

from __future__ import (absolute_import, division, print_function,
                        unicode_literals)

import numpy as np
from numpy.random import randn, random, uniform, seed
import scipy.stats


def generateParticles(x_range, y_range, hdg_range, N):
    
    # Uniformly generate particles upon initialisation.

    particles = np.empty((N, 3))
    particles[:, 0] = uniform(x_range[0], x_range[1], size=N)
    particles[:, 1] = uniform(y_range[0], y_range[1], size=N)

    # Heading of the particle, mod to get in radians

    particles[:, 2] = uniform(hdg_range[0], hdg_range[1], size=N)
    particles[:, 2] %= 2 * np.pi

    return particles


def predict(particles, u, std, dt, N):

    # Predict movement based off earlier movements (u: heading, velocity).

    particles[:, 2] += u[0] + randn(N) * std[0]
    particles[:, 2] %= 2 * np.pi

    d = u[1] * dt + randn(N) * std[1]
    particles[:, 0] += np.cos(particles[:, 2]) * d
    particles[:, 1] += np.sin(particles[:, 2]) * d


def update(particles, weights, sample, std_err, static_nodes):
    
    update_dists = np.linalg.norm(static_nodes - sample, axis=1)    

    weights[:] = 1.

    for i, static_node in enumerate(static_nodes):
        distance = np.linalg.norm(particles[:, 0:2] - static_node, axis=1)
        weights *= scipy.stats.norm(distance, std_err).pdf(update_dists[i])

    weights += 1.e-300
    weights /= sum(weights)


def resample(particles, weights, N):
    
    cumulative_sum = np.cumsum(weights)
    cumulative_sum[-1] = 1. # avoid round-off error
    indexes = np.searchsorted(cumulative_sum, random(N))

    # resample according to indexes
    particles[:] = particles[indexes]

    weights = 1. / N

def estimate(particles, N):

    x = sum(particles[:, 0]) / N
    y = sum(particles[:, 1]) / N

    return x, y
