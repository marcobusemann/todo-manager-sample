#include <QMetaOrm/MetaEntity.h>
#include <data/todo.h>

namespace QMetaOrm {
    namespace Mappings {
        template <>
        MetaEntity::Ptr mapping<Todo>();
    }
}
