#include "usd_writer_abstract.h"

#include <pxr/base/tf/stringUtils.h>

USDAbstractWriter::USDAbstractWriter(pxr::UsdStageRefPtr stage,
                                     const pxr::SdfPath &parent_path,
                                     Object *ob_eval,
                                     const DEGObjectIterData &degiter_data)
    : m_stage(stage), m_parent_path(parent_path), m_object(ob_eval), m_degiter_data(degiter_data)
{
  std::string usd_name(pxr::TfMakeValidIdentifier(ob_eval->id.name + 2));
  m_path = m_parent_path.AppendPath(pxr::SdfPath(usd_name));
}

USDAbstractWriter::~USDAbstractWriter()
{
}

const pxr::SdfPath &USDAbstractWriter::usd_path() const
{
  return m_path;
}

bool USDAbstractWriter::is_supported() const
{
  return true;
}

void USDAbstractWriter::write()
{
  do_write();
}
