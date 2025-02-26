using Pkg
Pkg.activate(".")

include("transceive_data.jl")

using AdaStress
import AdaStress.BlackBox
using Plots

using Distributions
using Random

const PORT_STRESS = 9900
const IP_ADDR = "localhost"

# AdaStress interface setup
Base.@kwdef mutable struct simple_vehicle <: AdaStress.BlackBox
	count::Int64 = 0
	count_max::Int64 = 10
	psi_dot::Float64 = 0.0 # the yaw-rate of the vehicle
	psi_dot_fail::Float64 = 10.0 # radians per second
	dist::Normal = Normal(0.0, 0.25)
	log::Vector{Float64} = []
end


function initialize!(sim::simple_vehicle)
	# In this example yaw rate is the only consideration and resetting the 
	# vehicle's entire state in the simple vehicle program is not implemented 
	# here. When resetting the __entire__ vehicle state, a function will need to 
	# be implemented that sends a "vehicle-state-reset-message" to the 
	# c-simulation that message not implemented yet
	sim.count = 0
	sim.psi_dot = 0.0
	sim.log = [sim.psi_dot]
	transceive_send_reset_cmd()
end

function update!(sim::simple_vehicle)
	sim.psi_dot = transceive_read_bytes_from_simulation()
    stress_value = Float32(rand(sim.dist))
	t = typeof(stress_value)
	transceive_send_float(stress_value)
    push!(sim.log, sim.psi_dot)
	
    sim.count = sim.count + 1
    return logpdf(sim.dist, stress_value) - logpdf(sim.dist, 0.0)
end

AdaStress.reset!(sim::simple_vehicle) = initialize!(sim)
AdaStress.step!(sim::simple_vehicle) = update!(sim)
AdaStress.isterminal(sim::simple_vehicle) = sim.count >= sim.count_max
AdaStress.isevent(sim::simple_vehicle) = abs(sim.psi_dot) >= sim.psi_dot_fail
AdaStress.distance(sim::simple_vehicle) = max(sim.psi_dot_fail - sim.psi_dot, 0.0)

println("In a separate terminal, run")
println("`./bin/main --kp 1.0 -v --speed 15.0 --dt 0.1 -p 1 --stress-test -a`")
println("or")
println("`./bin/main --kp 1.0 -v --speed 15.0 --dt 0.1 -p 1 --stress-test`")
sim = simple_vehicle()

transceive_init(IP_ADDR, PORT_STRESS)

mcs = AdaStress.Solvers.MCS(num_iterations=2000)

mdp = AdaStress.ASTMDP(simple_vehicle())
mdp.reward.event_bonus = 100.0

Random.seed!(0)
@time sol = mcs(() -> mdp)

p = plot(legend=false)
for (result, r) in sol
    AdaStress.replay!(mdp, result)
    plot!(p, mdp.sim.log, xlabel="Number of Steps to Failure", ylabel="Yaw Rate (rad/s)")
    
end
p