CREATE VIEW su_plus_machine_ip as select su_1.suid, machines.machine_ipv4 FROM su_1, machines WHERE su_1.suid = machines.suid;


CREATE VIEW machine_plus_os AS SELECT machines.machine_id, machines.os, os_1.osname FROM machines, os_1 WHERE machines.os = os_1.osid;




