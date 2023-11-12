import numpy as np
from math import acos
from typing import Tuple

class LineOfSight:
    def __init__(self, los):
        self.los = np.asarray(los)

def calculate_phase_angle(los: LineOfSight, positions: np.ndarray) -> np.ndarray:
    """
    Calculate phase angle between line of sight 
    and multiple positions.

    Parameters
    ----------
    los : LineOfSight
        Line of sight direction
    positions : np.ndarray
        (N, 3) array of XYZ positions
    
    Returns
    -------
    np.ndarray
        Phase angles in radians
    
    """

    los_vector = np.asarray(los.los)
    
    if positions.ndim == 1:
        # Single position
        return acos(np.dot(-positions, -los_vector) /
                    np.linalg.norm(positions))
    
    # Multiple positions
    result = np.empty(len(positions))
    for i, pos in enumerate(positions):
        result[i] = acos(np.dot(-pos, -los_vector) / 
                         np.linalg.norm(pos))
        
    return result

def main(los: LineOfSight, positions: Tuple[float, np.ndarray]) -> np.ndarray:
    pos_array = np.asarray(positions)
    angles = calculate_phase_angle(los, pos_array)
    
    print(f"Phase angles: {angles}")
    
    return angles
