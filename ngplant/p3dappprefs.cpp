/***************************************************************************

 Copyright (C) 2008  Sergey Prokhorchuk

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

***************************************************************************/

#include <p3dwxcurvectrl.h>

#include <p3dappprefs.h>

static const wxChar    *ExportHiddenGroupsPath      = wxT("Export/ExportHiddenGroups");
static const wxChar    *ExportOutVisRangeGroupsPath = wxT("Export/ExportOutVisRangeGroups");

                   P3DExport3DPrefs::P3DExport3DPrefs
                                      ()
 {
  SetDefaults();
 }

unsigned int       P3DExport3DPrefs::LongToMode
                                      (long                Value)
 {
  if ((Value == P3D_ALWAYS) || (Value == P3D_NEVER)  || (Value == P3D_ASK))
   {
    return(Value);
   }
  else
   {
    return(P3D_ASK);
   }
 }

void               P3DExport3DPrefs::Read
                                      (const wxConfigBase *Config)
 {
  long             CfgParamLong;

  SetDefaults();

  if (Config->Read(ExportHiddenGroupsPath,&CfgParamLong))
   {
    HiddenGroupsExportMode = LongToMode(CfgParamLong);
   }

  if (Config->Read(ExportOutVisRangeGroupsPath,&CfgParamLong))
   {
    OutVisRangeExportMode = LongToMode(CfgParamLong);
   }
 }

void               P3DExport3DPrefs::Save
                                      (wxConfigBase       *Config) const
 {
  Config->Write(ExportHiddenGroupsPath,(long)HiddenGroupsExportMode);
  Config->Write(ExportOutVisRangeGroupsPath,(long)OutVisRangeExportMode);
 }

void               P3DExport3DPrefs::SetDefaults
                                      ()
 {
  HiddenGroupsExportMode = P3D_ASK;
  OutVisRangeExportMode  = P3D_ASK;
 }

static const wxChar    *RotationModePath  = wxT("/3DView/RotationMode");
static const wxChar    *MouseRotXSensPath = wxT("/3DView/MouseRotXSens");
static const wxChar    *MouseRotYSensPath = wxT("/3DView/MouseRotYSens");

                   P3DCameraControlPrefs::P3DCameraControlPrefs
                                           ()
 {
  Init();
 }

void               P3DCameraControlPrefs::Init
                                      ()
 {
  MouseRotYCS   = false;
  MouseRotXSens = 0.02;
  MouseRotYSens = 0.02;
 }

void               P3DCameraControlPrefs::Read
                                      (const wxConfigBase *Config)
 {
  long             ParamLong;
  double           ParamDouble;

  Init();

  if (Config->Read(RotationModePath,&ParamLong) &&
      (ParamLong >= 0) && (ParamLong < 2))
   {
    if (ParamLong == 0)
     {
      MouseRotYCS = false; /* turntable */
     }
    else
     {
      MouseRotYCS = true; /* trackball */
     }
   }

  if (Config->Read(MouseRotXSensPath,&ParamDouble))
   {
    MouseRotXSens = (float)ParamDouble;
   }

  if (Config->Read(MouseRotYSensPath,&ParamDouble))
   {
    MouseRotYSens = (float)ParamDouble;
   }
 }

void               P3DCameraControlPrefs::Save
                                      (wxConfigBase       *Config) const
 {
  Config->Write(RotationModePath,(long)(MouseRotYCS ? 1 : 0));
  Config->Write(MouseRotXSensPath,MouseRotXSens);
  Config->Write(MouseRotYSensPath,MouseRotYSens);
 }

static const wxChar    *GroundVisiblePath   = wxT("/3DView/GroundVisible");
static const wxChar    *GroundColorPath     = wxT("/3DView/GroundColor");
static const wxChar    *BackgroundColorPath = wxT("/3DView/BackgroundColor");

                   P3D3DViewPrefs::P3D3DViewPrefs
                                      ()
 {
  SetDefaults();
 }

void               P3D3DViewPrefs::Read
                                      (const wxConfigBase *Config)
 {
  wxString         ColorStr;
  long             CfgParamLong;

  SetDefaults();

  if (Config->Read(GroundVisiblePath,&CfgParamLong))
   {
    GroundVisible = (bool)CfgParamLong;
   }

  if (Config->Read(GroundColorPath,&ColorStr))
   {
    ColorFromStr(&GroundColor,ColorStr);
   }

  if (Config->Read(BackgroundColorPath,&ColorStr))
   {
    ColorFromStr(&BackgroundColor,ColorStr);
   }
 }

void               P3D3DViewPrefs::Save
                                      (wxConfigBase       *Config) const
 {
  Config->Write(GroundVisiblePath,(long)GroundVisible);
  Config->Write(GroundColorPath,ColorToStr(&GroundColor));
  Config->Write(BackgroundColorPath,ColorToStr(&BackgroundColor));
 }

void               P3D3DViewPrefs::SetDefaults
                                      ()
 {
  GroundVisible   = true;
  GroundColor     = P3DAppColor3b(0x80,0x80,0x80);
  BackgroundColor = P3DAppColor3b(0x00,0x00,0x00);
 }

bool               P3D3DViewPrefs::ColorFromStr
                                      (P3DAppColor3b      *Color,
                                       const wxString     &Str)
 {
  long R,G,B;

  if (Str.Length() != 6)
   {
    return(false);
   }

  if ((Str.Mid(0,2).ToLong(&R,16)) &&
      (Str.Mid(2,2).ToLong(&G,16)) &&
      (Str.Mid(4,2).ToLong(&B,16)))
   {
    Color->R = (unsigned char)R;
    Color->G = (unsigned char)G;
    Color->B = (unsigned char)B;

    return(true);
   }
  else
   {
    return(false);
   }
 }

wxString           P3D3DViewPrefs::ColorToStr
                                      (const P3DAppColor3b*Color)
 {
  wxString         Result;

  Result.Printf(wxT("%02X%02X%02X"),
                (unsigned int)Color->R,
                (unsigned int)Color->G,
                (unsigned int)Color->B);

  return(Result);
 }


static const wxChar    *UICurveCtrlWidthPath  = wxT("/UI/CurveCtrl/Width");
static const wxChar    *UICurveCtrlHeightPath = wxT("/UI/CurveCtrl/Height");

void               P3DUIControlsPrefs::Read
                                      (const wxConfigBase *Config)
 {
  long             ParamLong;

  if (Config->Read(UICurveCtrlWidthPath,&ParamLong) &&
      (ParamLong >= P3DCurveCtrlMinWidth))
   {
    P3DCurveCtrl::BestWidth = (unsigned int)ParamLong;
   }

  if (Config->Read(UICurveCtrlHeightPath,&ParamLong) &&
      (ParamLong >= P3DCurveCtrlMinHeight))
   {
    P3DCurveCtrl::BestHeight = (unsigned int)ParamLong;
   }
 }

void               P3DUIControlsPrefs::Save
                                      (wxConfigBase       *Config)
 {
  Config->Write(UICurveCtrlWidthPath,(long)P3DCurveCtrl::BestWidth);
  Config->Write(UICurveCtrlHeightPath,(long)P3DCurveCtrl::BestHeight);
 }

static const wxChar    *RenderQuirksUseColorArrayPath  = wxT("/RenderQuirks/UseColorArray");

                   P3DRenderQuirksPrefs::P3DRenderQuirksPrefs
                                      ()
 {
  SetDefaults();
 }

void               P3DRenderQuirksPrefs::Read
                                      (const wxConfigBase *Config)
 {
  long             ParamLong;

  SetDefaults();

  if (Config->Read(RenderQuirksUseColorArrayPath,&ParamLong))
   {
    UseColorArray = ParamLong;
   }
 }

void               P3DRenderQuirksPrefs::Save
                                      (wxConfigBase       *Config) const
 {
  Config->Write(RenderQuirksUseColorArrayPath,(long)(UseColorArray ? 1 : 0));
 }

void               P3DRenderQuirksPrefs::SetDefaults
                                      ()
 {
  UseColorArray = false;
 }

