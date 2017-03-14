#include "modelbuilder.h"
#include "decorators\headerdatamodeldecorator.h"

//-----------------------------------------------------------------------------
ModelBuilderCommon ModelBuilderData::withHandler(DataModelDecorator::Handler handler)
{
    QAbstractProxyModel *model = nullptr;
    if (m_column == -1 && m_role == -1)
        model = new DataModelDecorator(m_exclusions, handler, m_parent);
    else if (m_role == -1)
        model = new ColumnBasedDataModelDecorator(m_column, m_exclusions, handler, m_parent);
    else if (m_column == -1)
        model = new RoleBasedDataModelDecorator(m_role, handler, m_parent);
    else
        model = new ColumnAndRoleBasedDataModelDecorator(m_column, m_role, handler, m_parent);

    model->setSourceModel(m_base);
    return ModelBuilderCommon(model, m_parent);
}

//-----------------------------------------------------------------------------
ModelBuilderCommon ModelBuilderHeaderData::withHandler(HeaderDataModelDecorator::Handler handler)
{
    QAbstractProxyModel *model = nullptr;
    if (m_section == -1 && m_role == -1)
        model = new HeaderDataModelDecorator(m_orientation, handler, m_parent);
    else if (m_role == -1)
        model = new SectionBasedHeaderDataModelDecorator(m_section, m_orientation, handler, m_parent);
    else if (m_section == -1)
        model = new RoleBasedHeaderDataModelDecorator(m_role, m_orientation, handler, m_parent);
    else
        model = new SectionAndRoleBasedHeaderDataModelDecorator(m_section, m_role, m_orientation, handler, m_parent);

    model->setSourceModel(m_base);
    return ModelBuilderCommon(model, m_parent);
}