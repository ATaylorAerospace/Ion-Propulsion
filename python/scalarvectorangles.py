import numpy as np
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
    los_norm = np.linalg.norm(los_vector)
    
    # Normalize the line of sight vector
    los_unit = los_vector / los_norm

    if positions.ndim == 1:
        # Single position
        positions_norm = np.linalg.norm(positions)
        pos_unit = positions / positions_norm
        return np.arccos(np.clip(-np.dot(pos_unit, los_unit), -1.0, 1.0))

    # Multiple positions
    positions_norms = np.linalg.norm(positions, axis=1, keepdims=True)
    positions_unit = positions / positions_norms
    dot_products = -np.dot(positions_unit, los_unit)
    return np.arccos(np.clip(dot_products, -1.0, 1.0))

def main(los: LineOfSight, positions: Tuple[float, np.ndarray]) -> np.ndarray:
    pos_array = np.asarray(positions)
    angles = calculate_phase_angle(los, pos_array)
    
    print(f"Phase angles: {angles}")
    
    return angles

