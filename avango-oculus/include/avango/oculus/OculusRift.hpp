#ifndef AVANGO_OCULUS_OCULUSRIFT_HPP
#define AVANGO_OCULUS_OCULUSRIFT_HPP

#if defined (_MSC_VER)
	#if defined (AV_OCULUS_LIBRARY)
		#define AV_OCULUS_DLL __declspec( dllexport )
	#else
		#define AV_OCULUS_DLL __declspec( dllimport )
	#endif
	#else
		#define AV_OCULUS_DLL
#endif

#include <gua/OculusRift.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/gua/renderer/Window.hpp>

#include <gua/math/math.hpp>
#include <avango/FieldContainer.h>

namespace av
{
	namespace oculus
	{
		/**
		* Wrapper for ::gua::OculusRift
		*/
		
		class AV_OCULUS_DLL OculusRift : public av::gua::Window
		{
			AV_FC_DECLARE();
			
			public:
			
				/**
				 * Constructor. When called without arguments, a new ::gua::OculusRift is created.
				 * Otherwise, the given ::gua::OculusRift is used.
				 */
#if WIN32
         OculusRift(::gua::OculusRift* guaOculusRift = new ::gua::OculusRift("\\\\.DISPLAY1"));
#else
				 OculusRift(::gua::OculusRift* guaOculusRift = new ::gua::OculusRift(":0.0"));
#endif
			 
			 protected:
			 
				 /**
				  * Destructor made protected to prevent allocation on stack.
				  */
				  
				 virtual ~OculusRift();
			 
			 public:
				 av::gua::SFMatrix Transform;
				 
				 virtual void getTransformCB(const av::gua::SFMatrix::GetValueEvent& event);
				 virtual void setTransformCB(const av::gua::SFMatrix::SetValueEvent& event);
			
				 /**
				  * Get the wrapped ::gua::OculusRift
				  */
				 ::gua::OculusRift* getGuaOculusRift() const;
			 
			 private:
				::gua::OculusRift* m_guaOculusRift;
				
				OculusRift(const OculusRift&);
				OculusRift& operator=(const OculusRift&);
		};
		
		typedef SingleField<Link<OculusRift> > SFOculusRift;
		typedef MultiField<Link<OculusRift> > MFOculusRift;
	}
	
#ifdef AV_INSTANTIATE_FIELD_TEMPLATES
  template class AV_OCULUS_DLL SingleField<Link<av::oculus::OculusRift> >;
  template class AV_OCULUS_DLL MultiField<Link<av::oculus::OculusRift> >;
#endif
}

#endif //AVANGO_OCULUS_OCULUSRIFT_HPP
