#ifndef AVANGO_OCULUS_OCULUSRIFT_HPP
#define AVANGO_OCULUS_OCULUSRIFT_HPP

#include <gua/OculusRift.hpp>

#include <avango/gua/Fields.hpp>
#include <avango/gua/renderer/Window.hpp>

#include <gua/math/math.hpp>
#include <avango/FieldContainer.h>

#include <avango/oculus/windows_specific.hpp>

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
				 
				 OculusRift(::gua::OculusRift* guaOculusRift = new ::gua::OculusRift(":0.0"));
				 static void initOVR();
			 
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
  template class AV_OCULUS_DLL SingleField<Link<oculus::OculusRift> >;
  template class AV_OCULUS_DLL MultiField<Link<oculus::OculusRift> >;
#endif
}

#endif //AVANGO_OCULUS_OCULUSRIFT_HPP
