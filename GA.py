import random

from PIL import Image
import numpy as np


class GA:
    def _init_(self, image):
        self.image = image.copy()
        if self.image.mode != 'L':
            self.image.convert('L')
        self.imgarray = np.array(self.image)
        self.pop_size = random.randint(50, 100)
        self.population = [random.randint(0, 255) in range(self.pop_size)]
        self.threshold = random.randint(0, 255)

    def fitness(self):
        ret = []
        for one in self.population:
            ret.append()