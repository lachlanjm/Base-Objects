from __future__ import annotations
from math import sqrt

class Quaternion:
    @classmethod
    def multiplication(q1: Quaternion, q2: Quaternion) -> Quaternion:
        return Quaternion(
            q2.w*q1.w - q2.x*q1.x - q2.y*q1.y - q2.z*q1.z,
            q2.w*q1.x + q2.x*q1.w - q2.y*q1.z + q2.z*q1.y,
            q2.w*q1.y + q2.x*q1.z + q2.y*q1.w - q2.z*q1.x,
            q2.w*q1.z - q2.x*q1.y + q2.y*q1.x + q2.z*q1.w
        )
    
    @classmethod
    def conjugate(q: Quaternion) -> Quaternion:
        return Quaternion(q.w, -q.x, -q.y, -q.z)
    
    def __init__(self, w: float, x: float, y: float, z: float):
        self.w: float = w
        self.x: float = x
        self.y: float = y
        self.z: float = z
    
    def copy(self) -> Quaternion:
        return Quaternion(self.w, self.x, self.y, self.z)
    
    def magnitude(self) -> float:
        return sqrt(
            self.w ** 2
            + self.x ** 2
            + self.y ** 2
            + self.z ** 2
        )
    
    def normalise(self):
        size = self.magnitude()
        self.w /= size
        self.x /= size
        self.y /= size
        self.z /= size
    