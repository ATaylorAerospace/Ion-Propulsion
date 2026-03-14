% Author: A Taylor | Purpose: L1 Lagrange point approximation | Ref: Goebel/Katz/Vallado
function r_L1 = lagrange_point_l1(m1_kg, m2_kg, R_m)
% LAGRANGE_POINT_L1 Approximate distance of the L1 Lagrange point from m2.
%
%   r_L1 = lagrange_point_l1(m1_kg, m2_kg, R_m)
%
%   Computes the approximate L1 Lagrange point location using the
%   restricted three-body approximation:
%
%   $$r_{L1} = R \left(\frac{m_2}{3\,m_1}\right)^{1/3}$$
%
%   Inputs:
%       m1_kg - Mass of the primary body (kg)
%       m2_kg - Mass of the secondary body (kg)
%       R_m   - Distance between the two bodies (m)
%
%   Outputs:
%       r_L1  - Distance of L1 from the secondary body (m)

    arguments
        m1_kg (1,1) double {mustBePositive}
        m2_kg (1,1) double {mustBePositive}
        R_m   (1,1) double {mustBePositive}
    end

    r_L1 = R_m * (m2_kg / (3.0 * m1_kg))^(1.0/3.0);
end
