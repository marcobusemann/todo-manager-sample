create table persons (
   id integer not null primary key autoincrement,
   name text not null,
   surname text not null
);

create table todos (
   id integer not null primary key autoincrement,
   id_owner integer references persons(id),
   title text not null,
   description text,
   end_date text,
   is_completed text default 'F' not null check (is_completed in ('T', 'F'))
);

create table todo_workers (
   id_todo integer not null references todos(id),
   id_person integer not null references person(id),
   primary key (id_todo, id_person)
);
