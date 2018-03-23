// This work is loosely based on Ian Millington's "Game Physics Engine Development" 
//
// A particle is the simplest object that can be simulated in the physics system.
// It has position data (no orientation data), along with velocity. It can be integrated forward through time, and have linear forces, and impulses applied to it. 
// This system allows defining the floating-point precision of the elements. It obviously won't work for integer types so don't use it in that way.
#include "llc_coord.h"			// for ::llc::SCoord2<>
#include "llc_error.h"			// for ::llc::error_t
#include "llc_array.h"		// for ::llc::array_view<>

#ifndef LLC_PARTICLE_H_29384923874
#define LLC_PARTICLE_H_29384923874

namespace llc 
{
#pragma pack(push, 1)
	template<typename _tElement>
	struct SParticle2Forces {
		typedef				::llc::SCoord2<_tElement>							TCoord;
		static constexpr	const _tElement										VelocityEpsilon								= 0.0001f;

							TCoord												AccumulatedForce							= {};
							TCoord												Acceleration								= {};	// A vector representing the speed in a given direction 
							TCoord												Velocity									= {};	// A vector representing the speed in a given direction 

		inline constexpr	bool												VelocityDepleted							()																					const	noexcept	{ return (Velocity + Acceleration).LengthSquared() < (VelocityEpsilon * VelocityEpsilon); }
		// This basically does Acceleration += (Force * 1 / Mass) and Velocity += (Acceleration * Time).
							void												IntegrateAccumulatedForce					(const _tElement inverseMass, const _tElement damping, const double timeElapsed)			noexcept	{
			Acceleration															+= AccumulatedForce * inverseMass;				// Calculate linear acceleration from force inputs.
			AccumulatedForce														= {};											// Clear this out now that we've used it already.

			Velocity																+= Acceleration * timeElapsed;					// Update linear velocity from both acceleration and impulse.
			Velocity																*= ::pow(damping, timeElapsed);					// Impose drag.
		}
	};

	template<typename _tElement>
	struct SParticle2 {
		// The member variables are organized such that matches the order in which they are used.
							::llc::SCoord2<_tElement>							Position									= {};	
							SParticle2Forces<_tElement>							Forces										= {};
							_tElement											InverseMass									= 0;
							_tElement											Damping										= .99f;	// A vector representing the speed in a given direction 

		inline				void												SetMass										(const double mass)																			noexcept	{ InverseMass = mass ? ((_tElement)(1.0 / mass)) : 0;		}
		inline constexpr	double												GetMass										()																					const	noexcept	{ return (InverseMass == 0) ? DBL_MAX : 1.0 / InverseMass;	}
		inline constexpr	bool												HasFiniteMass								()																					const	noexcept	{ return InverseMass >= 0.0f;								}
	};

	// This compact structure allows to define all the boolean states of the particle packed in a single byte.
	struct SParticle2State {
							bool												Unused										: 1;
							bool												Active										: 1;

		inline constexpr	bool												RequiresProcessing							()																					const	noexcept	{ return (false == Unused) && Active; }
	};
#pragma pack(pop)
	// This basically does FinalPosition = InitialPosition + Velocity * Time.
	template<typename _tElement>
	static inline		void												particleIntegratePosition
		(	const ::llc::SCoord2<_tElement>	& velocity
		,	const double					timeElapsed
		,	const double					timeElapsedHalfSquared
		,	::llc::SCoord2<_tElement>		& position
		)
	{
		position																+= velocity * timeElapsed;
		position																+= velocity * timeElapsedHalfSquared;
	}

	template<typename _tElement>
						::llc::error_t										integrate									(const ::llc::array_view<::llc::SParticle2<_tElement>>& particles, ::llc::array_pod<::llc::SParticle2State>& particleStates, ::llc::array_view<::llc::SParticle2<_tElement>>& particlesNext, double timeElapsed, double timeElapsedHalfSquared)			{
		for(uint32_t iParticle = 0, particleCount = (uint32_t)particleStates.size(); iParticle < particleCount; ++iParticle)	
			if(particleStates[iParticle].RequiresProcessing()) {
				::llc::SParticle2<_tElement>												& particleNext								= particlesNext[iParticle] = particles[iParticle];	// Copy the current particle state to the next
				::llc::particleIntegratePosition(particleNext.Forces.Velocity, timeElapsed, timeElapsedHalfSquared, particleNext.Position);
				particleNext.Forces.IntegrateAccumulatedForce(particleNext.InverseMass, particleNext.Damping, timeElapsed);
				if(particleNext.Forces.VelocityDepleted())
					particleStates[iParticle].Active										= false;
			}
		return 0;
	}

	template<typename _tElement>
	struct SParticle2Integrator {
		typedef				::llc::SParticle2	<_tElement>						TParticle;
		typedef				::llc::SCoord2		<_tElement>						TCoord;

							::llc::array_pod<::llc::SParticle2State>			ParticleState								= {};
							::llc::array_pod<TParticle>							Particle									= {};
							::llc::array_pod<TParticle>							ParticleNext								= {};
		// -----------------------------------------------------------------	---
		inline				::llc::error_t										Integrate									(double timeElapsed, double timeElapsedHalfSquared)														{ return Integrate(ParticleNext, timeElapsed, timeElapsedHalfSquared);	}
							::llc::error_t										Integrate									(::llc::array_view<TParticle>& particleNext, double timeElapsed, double timeElapsedHalfSquared)			{ return ::llc::integrate(Particle, ParticleState, particleNext, timeElapsed, timeElapsedHalfSquared);		}
		// --------------------------------------------------------------------
							::llc::error_t										AddParticle									(const TParticle& particleData)																			{
								const uint32_t											particleCount								= (uint32_t)ParticleState.size();
			static constexpr	const ::llc::SParticle2State							initialParticleState						= {false, true};

			for(uint32_t iBody = 0; iBody < particleCount; ++iBody)	// Check if there is any unused particle that we can recycle.
				if( ParticleState	[iBody].Unused ) {
					ParticleState	[iBody]													= initialParticleState;
					Particle		[iBody]													= 
					ParticleNext	[iBody]													= particleData;
					return iBody;
				}
			try {	// Later on we're going to add a way to avoid using ::std::vector which require these ugly try/catch blocks.
				ParticleState		.push_back(initialParticleState);
				Particle			.push_back(particleData);
				ParticleNext		.push_back(particleData);
			}
			catch(...) { // Restore the previous size to all the containers and return an error.
				ParticleState		.resize(particleCount);
				Particle			.resize(particleCount);
				ParticleNext		.resize(particleCount);
				return -1;
			}
			return particleCount;
		}
	};

	template<typename _tParticleType>
	struct SParticleBinding {
		typedef					_tParticleType									TParticleType;

								TParticleType									Binding										= {};
								int32_t											IndexParticlePhysics						= -1;
	};

	template<typename _tParticleType, typename _tCoord>
							::llc::error_t									addParticle														
		(	const _tParticleType										& particleType
		,	::llc::array_pod<::llc::SParticleBinding<_tParticleType>>	& particleInstances
		,	::llc::SParticle2Integrator<_tCoord>						& particleIntegrator
		,	const ::llc::SParticle2<_tCoord>							& particleDefinition
		)														
	{
		::llc::SParticleBinding<_tParticleType>									newInstance									= {}; 
		newInstance.Binding													= particleType; 
		llc_necall(newInstance.IndexParticlePhysics = particleIntegrator.AddParticle(particleDefinition), "Failed to add particle definition instance to integrator."); 
		return particleInstances.push_back(newInstance);
	}

	template<typename _tParticleType, typename _tCoord>
	struct SParticleSystem {
		typedef					::llc::SParticleBinding<_tParticleType>		TParticleInstance;
		typedef					::llc::SParticle2Integrator<_tCoord>			TIntegrator;

								::llc::array_pod<TParticleInstance>				Instances									= {};
								TIntegrator										Integrator									= {};
	};

	template<typename _tParticleType, typename _tCoord>
	static inline			::llc::error_t									addParticle									(const _tParticleType& particleType, ::llc::SParticleSystem<_tParticleType, _tCoord> & particleSystem,	const ::llc::SParticle2<_tCoord> & particleDefinition)	{
		return addParticle(particleType, particleSystem.Instances, particleSystem.Integrator, particleDefinition);
	}
} // namespace

#endif // LLC_PARTICLE_H_29384923874