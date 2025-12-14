from __future__ import annotations
from math import sqrt

class Vector:
    def __init__(self, x: float, y: float, z: float):
        self.x: float = x
        self.y: float = y
        self.z: float = z
    
    def copy(self) -> Vector:
        return Vector(self.x, self.y, self.z)
    
    def add(self, vector: Vector) -> None:
        self.x += vector.x
        self.y += vector.y
        self.z += vector.z
    
    def magnitude(self) -> float:
        return sqrt(
            self.x ** 2
            + self.y ** 2
            + self.z ** 2
        )
    
    def normalise(self) -> None:
        size = self.magnitude()
        self.x /= size
        self.y /= size
        self.z /= size
    
    def dotProduct(self, vector: Vector) -> float:
        return (
            self.x * vector.x
            + self.y * vector.y
            + self.z * vector.z
        )
    
    def cosThetaAngleDiff(self, vector: Vector) -> float:
        """Calculates the cos theta angle diference between the two vectors. I.e. cos(theta) = a.b / ( ||a||*||b|| )

        Args:
            vector (Vector): the other vector

        Returns:
            float: the resulting cos theta angle
        """
        #TODO make faster?
        return self.dotProduct(vector) / (self.magnitude()*vector.magnitude())
    